#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void * upMalloc(size_t size)
{
	//rise privilege
	__asm__ volatile("svc #0":::);
	void * retPtr = malloc(size);
	//fall privilege
	__asm__ volatile(
			"mrs r0, control	\n"
			"orr r0, #1			\n"
			"msr control, r0	\n"
			:
			:
			:"r0");
	return retPtr;
}

void upFree (void * ptr)
{
	//rise privilege
	__asm__ volatile("svc #0":::);
	free(ptr);
	//fall privilege
	__asm__ volatile(
			"mrs r0, control	\n"
			"orr r0, #1			\n"
			"msr control, r0	\n"
			:
			:
			:"r0");
}


caddr_t _sbrk(int incr)
{
	extern char uphstart asm("_uphstart");
	extern char uphend asm("_uphend");

	static char * 	curr_heap_end;
	char * 			prev_heap_end;

	if (curr_heap_end == 0)
		curr_heap_end = &uphstart;

	prev_heap_end = curr_heap_end;
	if (curr_heap_end + incr > &uphend)
	{
		return (caddr_t) -1;
	}
	curr_heap_end += incr;
	return (caddr_t) prev_heap_end;
}

#ifdef __cplusplus
}
#endif
