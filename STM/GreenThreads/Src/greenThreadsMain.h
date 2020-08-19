#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* Your cpp function should be exported to C main from here */

int test();

/*!
 * Incompatible with freeRTOS
 * in freeRTOS this handler is defined as 'vPortSVCHandler'
 */
void SVC_Handler(void);
void SVC_Handler_C(void);

#ifdef __cplusplus
}
#endif
