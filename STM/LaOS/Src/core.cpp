#include "core.hpp"
#include "mpu_armv7.h"

#if PROTECTED_STACK
//TODO : remove to separate memory location
__attribute__((section (".unprivileged_stack")))
uint32_t stackPool[PROTECTION_ZONE_WORDS + \
	(THREAD_STACK_SIZE_WORDS  + PROTECTION_ZONE_WORDS) * THREAD_AMNT]  \
	__attribute__((aligned (PROTECTION_ZONE_WORDS * 4)));
#endif

using namespace LaOS;

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

uint8_t getRegionSize(uint32_t size)
{
	uint8_t regionSize = 0;
	while(size)
	{
		size /= 2;
		regionSize++;
	}
	switch(regionSize)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			return ARM_MPU_REGION_SIZE_32B;
		case 5:
			return ARM_MPU_REGION_SIZE_64B;
		case 6:
			return ARM_MPU_REGION_SIZE_64B;
		case 7:
			return ARM_MPU_REGION_SIZE_128B;
		case 8:
			return ARM_MPU_REGION_SIZE_256B;
		case 9:
			return ARM_MPU_REGION_SIZE_512B;
		case 10:
			return ARM_MPU_REGION_SIZE_1KB;
		case 11:
			return ARM_MPU_REGION_SIZE_2KB;
		case 12:
			return ARM_MPU_REGION_SIZE_4KB;
		case 13:
			return ARM_MPU_REGION_SIZE_8KB;
		case 14:
			return ARM_MPU_REGION_SIZE_16KB;
		case 15:
			return ARM_MPU_REGION_SIZE_32KB;
		case 16:
			return ARM_MPU_REGION_SIZE_64KB;
		default : /* Actually there are the same numbers under this definitions */
			return regionSize;
	}
}

void Core::ConfigMPU()
{
	/* Get .text section addresses */
	uint32_t upCodeAddrStart = 0;
	uint32_t upCodeAddrEnd = 0;
	__asm__ volatile ("ldr %[addr], =_stext"
			:[addr]"=r"(upCodeAddrStart)
			:
			:"r0");
	__asm__ volatile ("ldr %[addr], =_etext"
		:[addr]"=r"(upCodeAddrEnd)
		:
		:"r0");

	/* Configure MMU for code - unprivileged read only */
	MPU->RNR = 0;
	MPU->RBAR = (uint32_t)upCodeAddrStart;
	uint8_t regionSize = getRegionSize(upCodeAddrEnd - upCodeAddrStart);
	MPU->RASR =
		0				<< MPU_RASR_ENABLE_Pos 	| /* bit 0 */
		regionSize		<< MPU_RASR_SIZE_Pos 	| /* bits 5:1 */
						/* Sub-region disable */
		0				<< MPU_RASR_SRD_Pos 	| /* bits 15:8 */
		1				<< MPU_RASR_B_Pos	 	| /* bufferable - bit 16 */
		1				<< MPU_RASR_C_Pos 		| /* cacheable - bit17 */
		1				<< MPU_RASR_S_Pos 		| /* shareable - bit18 */
						/* Type extension field */
		0				<< MPU_RASR_TEX_Pos 	| /* bits 21:19 */
						/* Unprivileged access read-only */
		ARM_MPU_AP_URO	<< MPU_RASR_AP_Pos		| /* bits 26:24 */
		1				<< MPU_RASR_XN_Pos		; /* bit 28*/

	/* Configure MMU for stack */
	MPU->RNR = 1;
	MPU->RBAR = (uint32_t)&stackPool[0];
	regionSize = getRegionSize(sizeof(stackPool));
	MPU->RASR =
		/* Disable instruction fetch */
		1 				<< MPU_RASR_ENABLE_Pos 	| /* bit 0 */
		regionSize 		<< MPU_RASR_SIZE_Pos 	| /* bits 5:1 */
		0 				<< MPU_RASR_SRD_Pos 	| /* bits 15:8 */
		1 				<< MPU_RASR_B_Pos	 	| /* bufferable - bit 16 */
		1 				<< MPU_RASR_C_Pos 		| /* cacheable - bit17 */
		1 				<< MPU_RASR_S_Pos 		| /* shareable - it18 */
		0 				<< MPU_RASR_TEX_Pos 	| /* bits 21:19 */
						/* Full access */
		ARM_MPU_AP_FULL << MPU_RASR_AP_Pos		| /* bits 26:24 */
		0				<< MPU_RASR_XN_Pos		; /* bit 28*/
}

/*!
 * In case w/out protected stack
 * it is responsibility of user
 * to provide usable stack and its size.
 * Current implementation not relies on 'malloc'
 * so it may not to be present in the system.
 */
void Core::Create( Context& context )
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
# if(0)
	if(context.threadNumber == 0) /* Configure stack pool prolog*/
	{
		ARM_MPU_SetRegionEx(
				0,
				(uint32_t)&stackPool[0],
			ARM_MPU_RASR(
				1, /* 1= disable instruction fetches */
				ARM_MPU_AP_PRIV, /* AccessPermission = privileged access only */
				0UL, /* TypeExtField */
				1UL, /* IsShareable */
				1UL, /* IsCacheable */
				1UL, /* IsBufferable */
				0x00UL, /* Sub-region disable field */
#  if   (PROTECTION_ZONE_WORDS == 32)
				ARM_MPU_REGION_SIZE_128B));
#  elif (PROTECTION_ZONE_WORDS == 64)
				ARM_MPU_REGION_SIZE_256B));
#  else
	#error "Sanity test failed."
#  endif
	} //end if

	/* First region is taken by start */
	if(context.threadNumber < (MPU_REGIONS_CNT-1))
	{
		ARM_MPU_SetRegionEx(
				context.threadNumber + 1,
				(uint32_t)(&context.stack[0] + context.stackSize),
			ARM_MPU_RASR(
				1, /* 1= disable instruction fetches */
				ARM_MPU_AP_PRIV, /* AccessPermission = privileged access only */
				0UL, /* TypeExtField */
				1UL, /* IsShareable */
				1UL, /* IsCacheable */
				1UL, /* IsBufferable */
				0x00UL, /* Sub-region disable field */
#  if   (PROTECTION_ZONE_WORDS == 32)
				ARM_MPU_REGION_SIZE_128B));
#  elif (PROTECTION_ZONE_WORDS == 64)
				ARM_MPU_REGION_SIZE_256B));
#  else
	#error "Sanity test failed."
#  endif //PROTECTION_ZONE_WORDS
	} //end if
# endif //STATIC_STACK_PROTECTION
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
	*(context.stack) = THREAD_CONTROL;
}

#if(PROTECTED_STACK)
void Core::Start()
{
	ConfigMPU();
	ARM_MPU_Disable();
	ARM_MPU_Enable( MPU_CTRL_PRIVDEFENA_Msk );
	asmStart((void *)&head);
}
#endif

void Yield()
{

	//TODO : rise access level to exclusive
	__asm__ volatile(
				"mrs r0, control	\n" //read control
				"bic r0, #1     	\n" //clear bit 1 - privileged mode
				"msr control, r0	\n" //write back control
				:
				:
				:"r0", "cc");
	::LaOS::Core& core = ::Core::getInstance();
	asmYield((void *)&(core.current), (void*)(core.current->next));
	//TODO : fall access level
	if(core.current != &core.head)
	{
		__asm__ volatile(
				"mrs r0, control	\n" //read control
				"orr r0, #1     	\n" //set bit 1 - un-privileged mode
				"msr control, r0	\n" //write back control
				:
				:
				:"r0", "cc");
	}
}


#if(PROTECTED_STACK)
void Core::Supervisor(Core * instance)
{
	for(;;)
	{
#if(STATIC_STACK_PROTECTION)
		/* Check stack protection here*/
	if(instance->threadCount > 7) //MPU doesn't cover all regions
		instance->CheckStack();
#endif

	#if(EVENT_DRIVEN) //sleep until new events
		__DSB();
		__ISB();
		__WFI();
	#endif
		Yield();
	}
}
#endif

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
			printf("Stack overflow in : %s\r\n", context->name);
			/* This disables all changes in stack
			 * and restore protection symbols */
			PrepareStack(*context);
		}
		//Check top
		if( stackPool [ PROTECTION_ZONE_WORDS + \
			(THREAD_STACK_SIZE_WORDS  + PROTECTION_ZONE_WORDS) * \
				context->threadNumber + context->stackSize] != PROTECTION_WORD )
		{
			printf("Stack corrosion in : %s\r\n", context->name);
			PrepareStack(*context); //whose stack was corroded
			if(context->next != &head)
				PrepareStack(*context->next); //who corroded stack
		}
	}
	//Check executor's stack
	assert( (headStack[0] == PROTECTION_WORD) \
			&& "Executor's stack overflow" );
}
/*!
 * Cause everything is allocated statically,
 * just remove from linked list;
 * @param context
 */
void Core::Kill( Context& context )
{
	/* Remove from linked list */
	Context * next = context.next;
	Context * prev = context.prev;
	next->prev = prev;
	prev->next = next;
}



