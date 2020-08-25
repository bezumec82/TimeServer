#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

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

extern int errno;
caddr_t _sbrk(int incr)
{
	extern char upHeapStart asm("_upHeapStart");
	extern char upHeapEnd   asm("_upHeapEnd");

	static char * 	curr_heap_end;
	char * 			prev_heap_end;

	if (curr_heap_end == 0)
		curr_heap_end = &upHeapStart;

	prev_heap_end = curr_heap_end;
	if (curr_heap_end + incr > &upHeapEnd)
	{
		errno = ENOMEM;
		return (caddr_t) -1;
	}
	curr_heap_end += incr;
	return (caddr_t) prev_heap_end;
}

#ifdef __cplusplus
}
#endif
