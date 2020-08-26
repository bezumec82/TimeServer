#include "core.hpp"

#if PROTECTED_STACK
__attribute__((section (".unprivileged_stack")))
uint32_t stackPool[PROTECTION_ZONE_WORDS + \
    (THREAD_STACK_SIZE_WORDS  + PROTECTION_ZONE_WORDS) * THREAD_AMNT]  \
    __attribute__(( aligned(32) ));
#endif

using namespace LaOS;

/*!
 * Singleton pattern
 */
Core& Core::getInstance()
{
    static Core core;
    return core;
}

Core::Core()
{
    /* Initialize head */
    head.name = "Head node/Idle thread";
    head.threadNumber = 0xbadf00d;
    /* Head context will be executed directly from 'main'
     * calling Yield will start it from main */
#if(PROTECTED_STACK)
    /* Prepare initial stack context */
    /* Descending full stack */
    /* Initial stack content must be :
     * pc -> r12..r0 -> control */
    head.stack = &headStack[0];
    head.stackSize = sizeof(headStack)/sizeof(uint32_t);
    //Place protection at bottom of stack - just in case
    head.stack[0] = PROTECTION_WORD;
    head.stack = &head.stack[0] + head.stackSize;
    /* Place pc */
    head.stack--;
    *(head.stack) =
        reinterpret_cast<uint32_t>(Supervisor);
    /* Let's pretend that r0-r12 are stacked at the beginning */
    head.stack -= REGS_AMNT;
    /* Place 'Supervisor' parameter to r0 */
    *(head.stack) = reinterpret_cast<uint32_t>(this);
    head.stack--;
    *(head.stack) = HEAD_CONTROL;
#endif
    /* Connect to itself */
    head.next = &head;
    head.prev = &head;
    /* Initial setting */
    current = &head;
}

/*!
 * In case w/out protected stack
 * it is the responsibility of the user
 * to provide usable stack and its size.
 * Current implementation not relies on 'malloc'
 * so it may not be present in the system.
 */
void Core::Create( Context& context )
{
#if(PROTECTED_STACK)

    if(threadCount == THREAD_AMNT)
    {
        PRINT_ERR("Thread amount is bigger than declared.\r\n");
        return;
    }
#else
    if(context.stackSize == 0) //TODO: less than min necessary amount
    {
        PRINTF("Stuck size is too low.\r\n");
        return;
    }
    ALIGN(context.stackSize);
#endif

    /* Should be done here - 'PrepareStack' is used for restart */
    context.threadNumber = threadCount;
    /* User must provide thread function, name and stack in some config */
    PrepareStack(context);

    /* Can be optimized, but will be less obvious */
    Context * last = head.prev;
    last->next = &context;
    context.next = &head;
    context.prev = last;
    head.prev = &context;
    threadCount++;
}

void Core::PrepareStack( Context& context )
{
#if (PROTECTED_STACK)
    /* The same formula will be used to allocate stack after thread fault */
    context.stack = &stackPool[0] + PROTECTION_ZONE_WORDS + \
    (THREAD_STACK_SIZE_WORDS  + PROTECTION_ZONE_WORDS) * context.threadNumber;
    //Place protection at bottom of stack
    context.stack[0] = PROTECTION_WORD;
    context.stackSize = THREAD_STACK_SIZE_WORDS;
#endif //PROTECTED_STACK

    /* Descending full stack */
    /* Prepare initial context */
    context.stack = &context.stack[0] + context.stackSize;
#if (PROTECTED_STACK)
    //Place protection at top of stack
    context.stack[0] = PROTECTION_WORD;
#endif
    context.stack--;
    /* pc at the very bottom of the stack */
    *(context.stack) =
            reinterpret_cast<uint32_t>(context.threadFunc);
    /* Let's pretend that r0-r12 are stacked at the beginning */
    context.stack -= REGS_AMNT;
    context.stack--;
#if(PROTECTED_STACK)
    *(context.stack) = context.threadLevel;
#else
    *(context.stack) = THREAD_PRIVILEGED;
#endif
}

void Yield()
{
    ::LaOS::Core& core = ::LaOS::Core::getInstance();
    asmYield((void *)&(core.current), (void*)(core.current->next));
}

#if(PROTECTED_STACK)
void Core::Start()
{
#if (USE_WATCHDOG)
    ConfigWDG();
#endif

#if ( (USE_SYSTICK) && (EVENT_DRIVEN_SYSTEM) )
    uint32_t ticks = (SystemCoreClock / 1000) * SYSTICK_PERIOD_MS;
    if( SysTick_Config(ticks) )
    {
        PRINT_ERR(  "Can't configure SysTick timer. \r\n"
                    "System will sleep forever.\r\n"
                    "ticks = %ld > %ld == SysTick_LOAD_RELOAD_Msk\r\n",
                    ticks, SysTick_LOAD_RELOAD_Msk);
        return;
    }
#endif

    ConfigMPU();
    asmStart((void *)&head);
}
void Core::Supervisor(Core * instance)
{
    for(;;)
    {
#if (USE_WATCHDOG)
    instance->KickDog();
#endif
#if(STATIC_STACK_PROTECTION)
    /* Check stack protection here*/
    instance->CheckStack();
#endif

    #if(EVENT_DRIVEN_SYSTEM) //sleep until new events
        PRINTF_COL(YEL, "Sleep.\r\n");
        HAL_SuspendTick();
        __DSB();
        __ISB();
        __SEV(); //Set event register if it is not set
        __WFE(); //Clear event register
        __WFE(); //Wait for event
        PRINTF_COL(GRN, "Awaken.\r\n");
        HAL_ResumeTick(); //for HAL library functionality
    #endif
        Yield();
    }
}

void Core::CheckStack()
{
    Context * context = head.prev; //do not check head
    //Check in reverse order - find overflow before corrosion
    for( ;context != &head; context = context->prev)
    {
        //Check bottom
        if ( stackPool [ PROTECTION_ZONE_WORDS + \
            (THREAD_STACK_SIZE_WORDS  + PROTECTION_ZONE_WORDS) * \
                context->threadNumber ] != PROTECTION_WORD )
        {
            PRINT_ERR("\r\nStack overflow in : '%s'\r\n", context->name);
            /* This disables all changes in stack
             * and restore protection symbols */
            PrepareStack(*context);
        }
        //Check top
        if( stackPool [ PROTECTION_ZONE_WORDS + \
            (THREAD_STACK_SIZE_WORDS  + PROTECTION_ZONE_WORDS) * \
                context->threadNumber + context->stackSize] != PROTECTION_WORD )
        {
            PRINT_ERR("\r\nStack corrosion in : %s\r\n", context->name);
            PrepareStack(*context); //whose stack was corroded
            if(context->next != &head)
                PrepareStack(*context->next); //who corroded stack
        }
    }
    //Check executor's stack
    assert( (headStack[0] == PROTECTION_WORD) \
            && "Core's stack overflow" );
}
#endif

/*!
 * Cause everything is allocated statically,
 * just remove from the linked list.
 * @param context - who to kill
 */
void Core::Kill( Context& context )
{
    /* Remove from linked list */
    Context * next = context.next;
    Context * prev = context.prev;
    next->prev = prev;
    prev->next = next;
}



