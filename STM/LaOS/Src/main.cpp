#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>

#include "core.hpp"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t stack1[128];
uint32_t stack2[128];
uint32_t stack3[256];
uint32_t testBuf[256];

char O = '0'; //global variable
void threadFunc1(void)
{
	char D = 'D'; //local variable
	for(;;)
	{
		const char * S = "S"; //scoped variable

		uint32_t arr[16];
		::std::fill_n(arr, 16, 0xdeadc0de);
		memcpy(testBuf, arr, sizeof(arr));

		printf("%s", &D);
		Yield();

		printf("%s", S);
		Yield();

		printf("%s", &O);
	}
}

void threadFunc2(void)
{
	char I = 'I'; //local variable
	for(;;)
	{
		const char * C = "C"; //scoped variable

		printf("%s", &I);
		Yield();

		printf("%s", C);
		Yield();

		printf("\r\n");
	}
}

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
	}
}

void stackOverflow(void)
{
	float a[16] = { M_PI };
	a[2] *= M_PI;
	Yield();
	a[1] = a[2];
	stackOverflow();
}

#if PROTECTED_STACK
int test()
{
	::LaOS::Core& core = ::LaOS::Core::getInstance();
	::LaOS::Context context1;
	context1.threadFunc = threadFunc1;
	context1.name = "Context 1";
	core.Create(context1);
#if(0)
	::LaOS::Context context2;
	context2.threadFunc = threadFunc2;
	context2.name = "Context 2";
	Core.Create(context2);
#endif
	::LaOS::Context floatContext;
	floatContext.threadFunc = threadFuncFloat;
	floatContext.name = "Float context";
	core.Create(floatContext);

	::LaOS::Context stackOverflowContext;
	stackOverflowContext.threadFunc = stackOverflow;
	stackOverflowContext.name = "Stack overflow context";
	core.Create(stackOverflowContext);

	core.Start();
	/* execution returns here after full circle */
	return EXIT_SUCCESS;
}

#else
int test()
{

	::LaOS::Context context1;
	context1.stack = &stack1[0];
	context1.stackSize = sizeof(stack1)/sizeof(uint32_t);
	context1.threadFunc = threadFunc1;
	context1.name = "Context 1";
	Core.Create(context1);

	::LaOS::Context context2;
	context2.stack = &stack2[0];
	context2.stackSize = sizeof(stack2)/sizeof(uint32_t);
	context2.threadFunc = threadFunc2;
	context2.name = "Context 2";
	Core.Create(context2);

	::LaOS::Context floatContext;
	floatContext.stack = &stack3[0];
	floatContext.stackSize = sizeof(stack3)/sizeof(uint32_t);
	floatContext.threadFunc = threadFuncFloat;
	floatContext.name = "Float context";
	Core.Create(floatContext);

	for(;;)
		Core.Yield();

	/* execution returns here after full circle */
	return EXIT_SUCCESS;
}
#endif




#ifdef __cplusplus
}
#endif
