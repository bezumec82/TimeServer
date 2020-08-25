#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h> //memset

#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

#include "memory.h"
#include "nmeaMsgExtractor.h"

/* Common part - out of abstraction */

#define UART_RX_TIMEOUT_MS 	20
#define UART_MAX_DELAY_MS	1000
#define UART_RX_BUF_SIZE    64

/*--- Structures definitions ---*/
struct UartData
{
	UART_HandleTypeDef * 	uartHandle;			/*!< UART in use */
	struct Node * 			msgList;			/*!< List of parsed messages */
	osSemaphoreId_t *		rxSemaHandle; 		/*!< 'void **' underneath */
};

/*--- Functions prototypes ---*/
void uartRxThread_func(struct UartData * uartData);
