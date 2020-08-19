#include "greenThreads.hpp"
#include <functional>

#if PROTECTED_STACK
//TODO : remove to separate memory location
uint32_t stackPool[PROTECTION_ZONE_WORDS + \
	(THREAD_STACK_SIZE  + PROTECTION_ZONE_WORDS) * THREAD_AMNT];
#endif


using namespace GreenThreads;

Engine::Engine()
{
	/* Initialize head */
	head.name = "Head node/Idle thread";
	head.threadNumber = threadCount;
	/* Head context will be executed directly from 'main'
	 * calling Yield will start it from main */
#if(PROTECTED_STACK)
	/* Prepare initial stack context */
	/* Descending full stack */
	/* Initial stack content must be :
	 * pc -> r13..r0 -> control */
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
 * it is responsibility of user
 * to provide usable stack and its size.
 * Current implementation not relies on 'malloc'
 * so it may not to be present in the system.
 */
void Engine::Create( Context& context )
{
#if(PROTECTED_STACK)

	if(threadCount == THREAD_AMNT)
	{
		PRINTF("Thread amount is bigger than declared.\r\n");
		return;
	}
#else
	if(context.stackSize == 0) //TODO: less than min necessary amount
	{
		PRINTF("Stuck size is too low.\r\n")
		return;
	}
	ALIGN(context.stackSize);
#endif
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

void Engine::PrepareStack( Context& context )
{
	context.threadNumber = threadCount;
#if (PROTECTED_STACK)
	/* The same formula will be used to allocate stack after thread fault */
	context.stack = &stackPool[0] + PROTECTION_ZONE_WORDS + \
	(THREAD_STACK_SIZE  + PROTECTION_ZONE_WORDS) * context.threadNumber;
	//Place protection at bottom of stack
	context.stack[0] = PROTECTION_WORD;
	context.stackSize = THREAD_STACK_SIZE;
#endif
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
	*(context.stack) = THREAD_CONTROL;
}

#if(PROTECTED_STACK)
void Engine::Start()
{
	asmStart((void *)&head);
}
#endif

void Engine::Yield()
{
	asmYield((void *)&current, (void*)current->next);
}

#if(PROTECTED_STACK)
void Engine::Supervisor(Engine * instance)
{
	for(;;)
	{
		/* Check stack protection here*/
		instance->CheckStack();

	#if(EVENT_DRIVEN) //sleep until new events
		__DSB();
		__ISB();
		__WFI();
	#endif
		instance->Yield();
	}
}
#endif

void Engine::CheckStack()
{
	Context * context = head.next; //do not check head
	for( ;context->next != &head; context = context->next)
	{
		//Check bottom
		assert( ( stackPool [ PROTECTION_ZONE_WORDS + \
			(THREAD_STACK_SIZE  + PROTECTION_ZONE_WORDS) * \
				context->threadNumber ] == PROTECTION_WORD ) \
					&& "Stack overflow");
		//Check top
		assert( ( stackPool [ PROTECTION_ZONE_WORDS + \
			(THREAD_STACK_SIZE  + PROTECTION_ZONE_WORDS) * \
				context->threadNumber + context->stackSize] == PROTECTION_WORD ) \
					&& "Stack corrosion");
	}
	//Check executor's stack
	assert( (headStack[0] == PROTECTION_WORD) \
			&& "Executor's stack overflow" );
}

