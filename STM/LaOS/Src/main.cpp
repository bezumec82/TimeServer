#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>

#include "core.hpp"
#include "atomic.hpp"
#include "debug.hpp"

#ifdef __cplusplus
extern "C" {
#endif

/*! No stack */
void threadFunc1(void)
{
    for(;;)
    {
        debug("D.\r\n");
        Yield();

        debug("S.\r\n");
        Yield();

        debug("0.\r\n");
        Yield();
    }
}

/*! Local and global variables */
void threadFunc2(void)
{
    const char * I = "I.\r\n"; //local variable
    for(;;)
    {
        const char * C = "C.\r\n"; //scoped variable
        debug(I);
        Yield();

        debug(C);
        Yield();

        debug("\r\n");
    } //end for
}

/*! This thread have float context.
 * Additional registers will be saved.
 * Look 'asmYield' in stepped mode. */
void threadFuncFloat(void)
{
    float pi = M_PI;
    float ret = 0.0;
    for(;;)
    {
        debug("Float context.\r\n");
        float local = 0.0;
        ret = pi * M_E;
        Yield();

        ret *= M_2_SQRTPI;
        Yield();

        local /= ret;
        ret += local;
    } //end for
}


/*!
 * This function show how allocation
 * in unprotected heap works.
 */
void allocTestFunc()
{
    for(;;)
    {
        /*--- Heap test ---*/
        char * test1 = (char *)upMalloc(256);
        strcpy(test1, "\r\nun-privileged heap test data 1\r\n");
        Yield();

        //this will fail because of 'malloc' internal
        //implementation uses global variables
        //char * test2 = (char *)malloc(512);

        char * test2 = (char *)upMalloc(512);
        strcpy(test2, "\r\nun-privileged heap test data 2\r\n");
        Yield();
        /* Check that data is pertained across calls */
        debug(test1);
        upFree(test1);
        Yield();
        debug(test2);
        upFree(test2);
        Yield();
    }
}

UNPRIVILEGED_DATA
::Atomic flag __attribute__(( aligned(4) )) = 5;
void atomicFunc1(void)
{
    for(;;)
    {
        uint32_t val = flag.load();
        flag.store(--val);
        Yield();
    }
}
void atomicFunc2(void)
{
    for(;;)
    {
        uint32_t val = flag.load();
        flag.store(++val);
        Yield();
    }
}

/*! Local stack overflow -
 * will be found be supervisor.
 * Thread will be restarted. */
void stackOverflow(void)
{
    float a[16] = { M_PI };
    a[2] *= M_PI;
    Yield();
    a[1] = a[2];
    stackOverflow();
}

char memFault[128] = {};    //global variable not accessible
                            //unless :
UNPRIVILEGED_BSS char memAccessible[128] = {};
UNPRIVILEGED_BSS int loopCounter = 0;
/*!
 * This function shows how MPU protects from
 * Data access violation
 */
void memFaultTestFunc()
{
    for(;;)
    {
        char arr[] = "Copy to unprotected global variable\r\n";
        memcpy(memAccessible, arr, strlen(arr));
        debug(&memAccessible[0]);
        Yield();
        loopCounter++;
        if(loopCounter == 10)
        {
            debug("\r\nMemory fault in ..3 ..2 ..1\r\n");
            memcpy(memFault, arr, strlen(arr));
        }
    } //end for
}

/*!
 * This function will be run in privileged
 * mode - it can access anything in memory space.
 */
void privlegedThread(void)
{
    printf("\r\nThis is privileged thread.\r\n"
            "It can use 'stdlib'.\r\n"
            "But be careful it has access to all resources of the core.");
    for(;;)
    {
        printf("Accessing protected region\r\n");
        memcpy(memFault, "Test", strlen("Test"));
        Yield();
    }
}


void wdgToutTestFunc()
{
    int loopCounter = 0;
    for(;;)
    {
        Yield();
        loopCounter++;
        if(loopCounter == 10)
        {
            debug("\r\nWDG bite us in ..3 ..2 ..1\r\n");
            while(1)
                ;
        }
    } //end for
}

#if PROTECTED_STACK
int test()
{
    ::LaOS::Core& core = ::LaOS::Core::getInstance();
    ::LaOS::Context context1;
    context1.threadFunc = threadFunc1;
    context1.name = "Context 1";
    context1.threadLevel = THREAD_UNPRIVILEGED;
    core.Create(context1);

    ::LaOS::Context context2;
    context2.threadFunc = threadFunc2;
    context2.name = "Context 2";
    context2.threadLevel = THREAD_UNPRIVILEGED;
    core.Create(context2);

    ::LaOS::Context floatContext;
    floatContext.threadFunc = threadFuncFloat;
    floatContext.name = "Float context";
    floatContext.threadLevel = THREAD_UNPRIVILEGED;
    core.Create(floatContext);

    ::LaOS::Context privContext;
    privContext.threadFunc = privlegedThread;
    privContext.name = "Privileged context";
    /* Change for un-privileged and look what happens. */
    //privContext.threadLevel = THREAD_PRIVILEGED;
    privContext.threadLevel = THREAD_PRIVILEGED;
    core.Create(privContext);

    ::LaOS::Context allocContext;
    allocContext.threadFunc = allocTestFunc;
    allocContext.name = "Memory allocation context";
    allocContext.threadLevel = THREAD_PRIVILEGED;
    core.Create(allocContext);

    ::LaOS::Context atomicContext1;
    atomicContext1.threadFunc = atomicFunc1;
    atomicContext1.name = "Atomic context 1";
    atomicContext1.threadLevel = THREAD_UNPRIVILEGED;
    core.Create(atomicContext1);

    ::LaOS::Context atomicContext2;
    atomicContext2.threadFunc = atomicFunc2;
    atomicContext2.name = "Atomic context 2";
    atomicContext2.threadLevel = THREAD_UNPRIVILEGED;
    core.Create(atomicContext2);
#if(0)/*--- Protection tests ---*/
    ::LaOS::Context stackOverflowContext;
    stackOverflowContext.threadFunc = stackOverflow;
    stackOverflowContext.name = "Stack overflow context";
    stackOverflowContext.threadLevel = THREAD_UNPRIVILEGED;
    core.Create(stackOverflowContext);
#endif

#if(0)
    ::LaOS::Context memFaultContext;
    memFaultContext.threadFunc = memFaultTestFunc;
    memFaultContext.name = "Memory fault context";
    memFaultContext.threadLevel = THREAD_UNPRIVILEGED;
    core.Create(memFaultContext);
#endif

#if ( (1) && (USE_WATCHDOG) )
    ::LaOS::Context wdgToutContext;
    wdgToutContext.threadFunc = wdgToutTestFunc;
    wdgToutContext.name = "Watchdog byte context";
    wdgToutContext.threadLevel = THREAD_UNPRIVILEGED;
    core.Create(wdgToutContext);
#endif
    printf("\r\nStarting core.\r\n");
    core.Start();
    /* execution returns here after full circle */
    return EXIT_SUCCESS;
}

#else
uint32_t stack1[128];
uint32_t stack2[128];
uint32_t stack3[256];
int test()
{
    ::LaOS::Core& core = ::LaOS::Core::getInstance();
    ::LaOS::Context context1;
    context1.stack = &stack1[0];
    context1.stackSize = sizeof(stack1)/sizeof(uint32_t);
    context1.threadFunc = threadFunc1;
    context1.name = "Context 1";
    core.Create(context1);

    ::LaOS::Context context2;
    context2.stack = &stack2[0];
    context2.stackSize = sizeof(stack2)/sizeof(uint32_t);
    context2.threadFunc = threadFunc2;
    context2.name = "Context 2";
    core.Create(context2);

    ::LaOS::Context floatContext;
    floatContext.stack = &stack3[0];
    floatContext.stackSize = sizeof(stack3)/sizeof(uint32_t);
    floatContext.threadFunc = threadFuncFloat;
    floatContext.name = "Float context";
    core.Create(floatContext);
    for(;;)
        Yield();
    return EXIT_SUCCESS;
}
#endif

#ifdef __cplusplus
}
#endif
