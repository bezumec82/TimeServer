#pragma once

#ifdef STM32H757xx
# include "stm32h7xx_hal.h"
#endif

#ifdef STM32F746xx
# include "stm32f7xx_hal.h"
#endif

#include <stdint.h>
#include <assert.h>
#include "cmsis_gcc.h"

#include "greenThreadsConfig.h"

#define TEST			true
#define PRINTF(text, ...)
#define ALIGN(size) 	size &= ( ~( sizeof(void*) - 1 ) );
#define REGS_AMNT		13 /* r0-r12 */

/* After reset, the CONTROL register is 0.
 * This means the Thread mode uses the Main Stack Pointer as Stack Pointer
 * and Thread mode has privileged accesses */
#define HEAD_CONTROL	0x0 //Privileged + main stack
/*
 * A program in unprivileged access level cannot switch itself
 * back to privileged access level.
 * So actually we can't work in un-privileged mode
 * without exception mechanism like in this implementation.
 */
#define THREAD_CONTROL	0x3	//UN-privileged + process stack



#define PROTECTION_WORD	0xdeadbeaf

extern "C"
{
void asmYield(void *, void *);
#if (PROTECTED_STACK)
void asmStart(void *);
#endif
}


namespace GreenThreads
{

	using ThreadFunc = void(*)();

	struct Context
	{
	friend class Engine;
	/* In case of protected stack,
	 * stack itself and its size defined inside this mechanism. */
#if (PROTECTED_STACK)
	private :
#endif
		uint32_t * stack; /*!< Pointer to the allocated stack */
	private :
		/* All context are connected in double-linked lists,
		 * can be optimized to single-linked */
		struct Context * next; //next is the !second! member of structure
		struct Context * prev;

#if (!PROTECTED_STACK)
	public :
#endif
		size_t stackSize; /*!< Stack size in words */
	public :
		const char * name;
		ThreadFunc threadFunc;
		uint32_t threadNumber; /*!< Used to find its place in stack pool */
	};

	class Engine
	{
	public : /*--- Constructors/Destructors ---*/
		Engine();

	public : /*--- Methods ---*/
		void Create( Context& );
		void Yield();
		void Start();
	private :
		static void Supervisor( Engine * );
		void PrepareStack( Context& );
		void CheckStack();
	private : /*--- Variables ---*/
		Context head = {};
		Context * current = nullptr;
#if(PROTECTED_STACK)
		uint32_t headStack[128];
#endif
		uint32_t threadCount = 0;
		const uint32_t DEFAULT_STACK_SIZE = 256;

	};

} //end namespace

