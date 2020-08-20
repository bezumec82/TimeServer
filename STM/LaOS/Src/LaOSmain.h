#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* Your cpp function should be exported to C code from here */

int test();

/*!
 * Incompatible with freeRTOS
 * in freeRTOS this handler is defined as 'vPortSVCHandler'
 */
void SVC_Handler(void); //reside in asm
void SVC_Handler_C(void);

#ifdef __cplusplus
}
#endif
