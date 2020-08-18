#include "greenThreads.hpp"
#include "stdlib.h"
#include "stdio.h"

::GreenThreads::Engine engine;

#ifdef __cplusplus
extern "C" {
#endif

uint32_t stack1[128];
char O = '0';
void threadFunc1(void)
{
	char D = 'D';
	for(;;)
	{
		const char * S = "S";

		printf("%s", &D);
		engine.Yeild();

		printf("%s", S);
		engine.Yeild();

		printf("%s", &O);
	}
}

uint32_t stack2[128];
void threadFunc2(void)
{
	char I = 'I';
	for(;;)
	{
		const char * C = "C";

		printf("%s", &I);
		engine.Yeild();

		printf("%s", C);
		engine.Yeild();

		printf("\r\n");
	}
}


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
	for(;;)
		engine.Yeild();

	/* execution returns here after full circle */

	return EXIT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
