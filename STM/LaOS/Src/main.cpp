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

UNPRIVILEGED_ATOMIC
::Atomic flag __attribute__(( aligned(4) )) = 5;

/*! No stack */
void threadFunc1(void)
{
	for(;;)
	{
/* 'stdio' internally uses variable 'errno' in global stack.
 * User should choose between protection and laziness. */
#if(!PROTECTED_STACK)
		printf("%s", &D);
#endif
		debug("D.");
		Yield();

		debug("S.");
		Yield();

		debug("0.");
		Yield();
		/* Atomic */
		uint32_t val = flag.load();
		flag.store(--val);
	}
}

char memFault[128] = {};	//global variable not accessible
					//unless :
REMOVE_PROTECTION
char memAccessible[128] = {};

REMOVE_PROTECTION int loopCounter = 0;
/*! Local and global variables */
void threadFunc2(void)
{
	const char * I = "I."; //local variable
	for(;;)
	{
		/*--- Heap test ---*/
		char * test1 = (char *)upMalloc(256);
		strcpy(test1, "\r\nun-privileged heap test data 1\r\n");
		char * test2 = (char *)upMalloc(512);
		strcpy(test2, "\r\nun-privileged heap test data 1\r\n");

		const char * C = "C."; //scoped variable
		debug(I);
		Yield();

		debug(C);
		Yield();

		debug("\r\n");

		char arr[] = "Copy to unprotected global variable\r\n";
		memcpy(memAccessible, arr, strlen(arr));
		debug(&memAccessible[0]);
		Yield();

		/* Check that data is pertained across calls */
		debug(test1);
		upFree(test1);
		debug(test2);
		upFree(test2);

		loopCounter++;
		if(loopCounter == 10)
		{
			debug("Memory fault in ..3 ..2 ..1\r\n");
			memcpy(memFault, arr, strlen(arr));
		}
	} //end for
}

/*! This thread have float context.
 * Additional registers will be saved. */
void threadFuncFloat(void)
{
	float pi = M_PI;
	float ret = 0.0;
	for(;;)
	{
		float local = 0.0;
		ret = pi * M_E;
		Yield();

		ret *= M_2_SQRTPI;
		Yield();

		local /= ret;
		ret += local;

		/* Atomic */
		uint32_t val = flag.load();
		flag.store(++val);

	} //end for
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

void privlegedThread(void)
{
	printf("\r\nThis is privileged thread.\r\n"
			"It can use 'stdlib'.\r\n"
			"But be careful it has access to all resources of the core.");
	for(;;)
	{
		printf("Yield from privileged thread.\r\n");
		Yield();
	}

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

	::LaOS::Context stackOverflowContext;
	stackOverflowContext.threadFunc = stackOverflow;
	stackOverflowContext.name = "Stack overflow context";
	stackOverflowContext.threadLevel = THREAD_UNPRIVILEGED;
	core.Create(stackOverflowContext);

	::LaOS::Context privContext;
	privContext.threadFunc = privlegedThread;
	privContext.name = "Privileged context";
	/* Change for un-privileged and look what happens. */
	privContext.threadLevel = THREAD_PRIVILEGED;
	core.Create(privContext);

	printf("Starting core.\r\n");
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

	/* execution returns here after full circle */
	return EXIT_SUCCESS;
}
#endif

#ifdef __cplusplus
}
#endif
