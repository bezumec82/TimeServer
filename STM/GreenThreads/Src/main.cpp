#include "greenThreads.hpp"
#include "stdlib.h"
#include "stdio.h"

::GreenThreads::Engine engine;

#ifdef __cplusplus
extern "C" {
#endif

uint32_t stack1[128];
uint32_t stack2[128];
uint32_t stack3[256];

char O = '0'; //global variable
void threadFunc1(void)
{
	char D = 'D'; //local variable
	for(;;)
	{
		const char * S = "S"; //scoped variable

		printf("%s", &D);
		engine.Yield();

		printf("%s", S);
		engine.Yield();

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
		engine.Yield();

		printf("%s", C);
		engine.Yield();

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
		engine.Yield();

		ret *= M_2_SQRTPI;
		engine.Yield();

		local /= ret;
		ret += local;
	}
}

#if PROTECTED_STACK
int test()
{
	::GreenThreads::Context context1;
	context1.threadFunc = threadFunc1;
	context1.name = "Context 1";
	engine.Create(context1);

	::GreenThreads::Context context2;
	context2.threadFunc = threadFunc2;
	context2.name = "Context 2";
	engine.Create(context2);

	::GreenThreads::Context floatContext;
	floatContext.threadFunc = threadFuncFloat;
	floatContext.name = "Float context";
	engine.Create(floatContext);

	engine.Start();
	/* execution returns here after full circle */
	return EXIT_SUCCESS;
}

#else
int test()
{

	::GreenThreads::Context context1;
	context1.stack = &stack1[0];
	context1.stackSize = sizeof(stack1)/sizeof(uint32_t);
	context1.threadFunc = threadFunc1;
	context1.name = "Context 1";
	engine.Create(context1);

	::GreenThreads::Context context2;
	context2.stack = &stack2[0];
	context2.stackSize = sizeof(stack2)/sizeof(uint32_t);
	context2.threadFunc = threadFunc2;
	context2.name = "Context 2";
	engine.Create(context2);

	::GreenThreads::Context floatContext;
	floatContext.stack = &stack3[0];
	floatContext.stackSize = sizeof(stack3)/sizeof(uint32_t);
	floatContext.threadFunc = threadFuncFloat;
	floatContext.name = "Float context";
	engine.Create(floatContext);

	for(;;)
		engine.Yield();

	/* execution returns here after full circle */
	return EXIT_SUCCESS;
}
#endif

void SVC_Handler_C(uint32_t svc_num)
{
	printf("SVC exception number : %ul", svc_num);
}


#ifdef __cplusplus
}
#endif
