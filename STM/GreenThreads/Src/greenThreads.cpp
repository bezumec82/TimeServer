#include "greenThreads.hpp"

using namespace GreenThreads;

Engine::Engine()
{
	/* Initialize head */
	head.stack = &idleStack[0];
	head.stackSize = sizeof(idleStack)/sizeof(uint32_t);
	head.threadFunc = idleTask;
	head.name = "Head node/Idle thread";

	/* Prepare initial context */
	head.stack = &head.stack[0] + head.stackSize;
	/* Full descendant stack */
	head.stack--;
	*(head.stack) =
			reinterpret_cast<uint32_t>(head.threadFunc); //set PC
	head.stack -= REGS_AMNT;

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
	if(context.stackSize == 0) //TODO: less than min necessary amount
	{
		PRINTF("Stuck size is too low.\r\n")
	}
	ALIGN(context.stackSize);
	/* Descending full stack */
	context.stack = &context.stack[0] + context.stackSize;

	/* Prepare initial context */
	context.stack--;
	*(context.stack) =
			reinterpret_cast<uint32_t>(context.threadFunc); //set PC
	context.stack -= REGS_AMNT;

	/* Can't be optimized, but will be less obvious */
	Context * last = head.prev;
	last->next = &context;
	context.next = &head;
	context.prev = last;
	head.prev = &context;

	threadCount++;
}

void Engine::Yeild()
{
#if(1)
	__asm__ (
		/* Get address of the next instruction after this function */
    	"push {lr}				\n"
//		"ldr r0, [%[context], #0]		\n" //get stack pointer in context structure
//		"str sp, [r0, #0]				\n" //save new stack pointer value
//		/* current = current->next */
//		"ldr r0, [r0, #4]				\n" //get next context
//		"str r0, [%[context], #0]		\n" //set current to the next context
//
//		"ldr sp, [%[context], #0]		\n" //set stack pointer for the next thread
		"pop {pc}				\n"
		/* bx lr - should be here*/
		: /* OutputOperands */
		: [context]"r"(&current) /* InputOperands */
		:"r0","r1","r2","r3","r4","r5","r6",
		"r8","r9","r10","r11","r12","cc","memory"
		);
#else
	asmYeild((void *)&current, (void*)current->next);
#endif
}

void Engine::idleTask()
{
#if(!TEST) /* */
	__DSB();
	__ISB();
	__WFI();
#else
	HAL_Delay(100);
#endif

}

