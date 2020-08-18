#pragma once

#ifdef STM32H757xx
# include "stm32h7xx_hal.h"
#endif

#ifdef STM32F746xx
# include "stm32f7xx_hal.h"
#endif

#include <stdint.h>
#include "cmsis_gcc.h"

#define TEST			true
#define PRINTF(text, ...)
#define ALIGN(size) 	size &= ( ~( sizeof(void*) - 1 ) );
#define REGS_AMNT		13 /* r0-r12 */

extern "C" void asmYeild(void *, void *);

namespace GreenThreads
{

	using ThreadFunc = void(*)();

	struct Context
	{
	friend class Engine;
		uint32_t * stack; /*!< Pointer to the allocated stack */
	private :
		struct Context * next; //next is the !second! member of structure
		struct Context * prev;

	public :
		size_t stackSize; /*!< Stack size in words */
		const char * name;
		ThreadFunc threadFunc;

	};

	class Engine
	{
	public : /*--- Constructors/Destructors ---*/
		Engine();

	public : /*--- Methods ---*/
		void Create( Context& context );
		void Yeild();

	private :
		static void idleTask();

	private : /*--- Variables ---*/
		Context head = {};
		Context * current = nullptr;
		uint32_t idleStack[64];
		uint32_t threadCount = 0;
		const uint32_t DEFAULT_STACK_SIZE = 256;

	};

} //end namespace

