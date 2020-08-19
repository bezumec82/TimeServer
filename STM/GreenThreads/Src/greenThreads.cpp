#include "greenThreads.hpp"

#if PROTECTED_STACK
uint32_t stackPool[PROTECTION_ZONE_WORDS + \
	(THREAD_STACK_SIZE  + PROTECTION_ZONE_WORDS) * THREAD_AMNT];
#endif


using namespace GreenThreads;

Engine::Engine()
{
	/* Initialize head */

	head.name = "Head node/Idle thread";

	/* Head context will be executed directly from 'main'
	 * calling Yield will start it from main */
#if(PROTECTED_STACK)
	head.stack = &headStack[0];
	head.stackSize = sizeof(headStack)/sizeof(uint32_t);
	head.threadFunc = Supervisor;
	PrepareStack(head);
#endif

	/* Connect to itself */
	head.next = &head;
	head.prev = &head;

	/* Initial setting */
	current = &head;
}

/*!
 * It is responsibility of user
 * to provide usable stack and its size.
 * Current implementation not relies on 'malloc'
 * so it may not to be present in the system.
 */
void Engine::Create( Context& context )
{
#if(PROTECTED_STACK)
	if(threadCount > THREAD_AMNT)
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
#endif
	ALIGN(context.stackSize);
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
#if (PROTECTED_STACK)
	if (&context != &head) //head has separate stack
	{
		context.stack = stackPool + PROTECTION_ZONE_WORDS + \
		(THREAD_STACK_SIZE  + PROTECTION_ZONE_WORDS) * threadCount;
	}
	context.stack[0] = PROTECTION_WORD;
#endif
	/* Descending full stack */
	context.stack = &context.stack[0] + context.stackSize;
	/* Prepare initial context */
	context.stack--;
	/* PC at the very bottom of the stack */
	*(context.stack) =
			reinterpret_cast<uint32_t>(context.threadFunc);
	/* Let's pretend that r0-r12 are stacked at the beginning */
	context.stack -= REGS_AMNT;
	context.stack--;
	if (&context != &head)
	{
		*(context.stack) = THREAD_CONTROL;
	}
#if (PROTECTED_STACK)
	else
	{
		*(context.stack) = HEAD_CONTROL;
	}
#endif
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
void Engine::Supervisor()
{
	/* Check stack protection here*/


#if(EVENT_DRIVEN) //sleep until new events
	__DSB();
	__ISB();
	__WFI();
#endif
	Yield();
}
#endif

