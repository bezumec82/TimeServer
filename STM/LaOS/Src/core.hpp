#pragma once

/* This is the key to all headers :
 * cmsis_gcc.h
 * cmsis_cm7.h
 * mpu_armv7.h
 */

#ifdef STM32H757xx
# include "stm32h7xx_hal.h"
#endif

#ifdef STM32F746xx
# include "stm32f7xx_hal.h"
#endif

#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "LaOSconfig.h"
#include "LaOSmain.h"

#include "wdg.hpp"
#include "debug.hpp"

#define TEST                true
#define ALIGN(size)         size &= ( ~( sizeof(void*) - 1 ) );
#define REGS_AMNT           8 /* r4-r11 */

/* After reset, the CONTROL register is 0.
 * This means the Thread mode uses the Main Stack Pointer as Stack Pointer
 * and Thread mode has privileged accesses */
#define HEAD_CONTROL            0x0 //Privileged + main stack
/* Un-privileged access will be set in yield */
#define THREAD_UNPRIVILEGED     0x3 //un-privileged + process stack
#define THREAD_PRIVILEGED       0x2
#define MPU_REGIONS_CNT         8
#define PROTECTION_WORD         0xdeadbeaf


extern "C"
{
void asmYield(void *, void *);
#if (PROTECTED_STACK)
void asmStart(void *);
void sysCall(int, ...);
#endif
}

extern void Yield();

namespace LaOS
{

    using ThreadFunc = void(*)();

    struct Context
    {
    friend class Core;
    friend void ::Yield();
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
#if(PROTECTED_STACK)
        uint32_t threadLevel;
#endif
    };

    class Core
    {
        friend void ::Yield();
        friend void ::MemManage_Handler(void); //to access current
    private : /*--- Constructors/Destructors ---*/
        Core();

    public : /*--- Methods ---*/
        static Core& getInstance();
        void Create( Context& );
        void Kill( Context& );
#if(PROTECTED_STACK)
        void Start();
#endif
#if(USE_WATCHDOG)
        void ConfigWDG();
        void KickDog();
#endif
    private :
        void PrepareStack( Context& );
#if(PROTECTED_STACK)
        static void Supervisor( Core * );
        void CheckStack();
        void ConfigMPU();
#endif
    private : /*--- Variables ---*/
        Context head = {};
        Context * current = nullptr;
#if(PROTECTED_STACK)
        uint32_t headStack[128];
#endif
        uint32_t threadCount = 0;
        const uint32_t DEFAULT_STACK_SIZE = 256;
#if(USE_WATCHDOG)
        IWDG_REGS * iwdgRegs =
            reinterpret_cast< IWDG_REGS * >(IWDG1_BASE);
        bool isIwdgConfigured = false;
#endif
    };

} //end namespace

