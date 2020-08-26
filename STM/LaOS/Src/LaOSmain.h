#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define UNPRIVILEGED_DATA   __attribute__(( section (".unprivileged_data") ))
#define UNPRIVILEGED_BSS    __attribute__(( section (".unprivileged_bss") ))

/* Your cpp function should be exported to C code from here */
int test();

/*!
 * Incompatible with freeRTOS
 * in freeRTOS this handler is defined as 'vPortSVCHandler'
 */
void SVC_Handler(void); //reside in asm
void SVC_Handler_C(uint32_t svc_num); //can be redefined
void MemManage_Handler(void);

/* Startup routines */
void zeroOutSection(uint32_t * , uint32_t * );
void copySection(uint32_t * ,
        uint32_t * , uint32_t * );

#include <stdio.h>
caddr_t _sbrk(int incr);
void * upMalloc(size_t size);
void upFree (void * ptr);

#ifdef __cplusplus
}
#endif
