#pragma once

#include <stdint.h>
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "circBuf.h"

/* Common part - out of abstraction */
#define UART_RX_CHUNK		32
#define UART_RX_TIMEOUT_MS 	1000

/*--- Structures definitions ---*/
struct UartData
{
	struct CircBuf * 		cBuf;				/*!< Circular buffer to store received data */
	UART_HandleTypeDef * 	uartHandle;			/*!< UART in use */
	osSemaphoreId_t *		rxSemaHandle; 		/*!< 'void **' underneath */
	osSemaphoreId_t * 		parseSemaHandle; 	/*!< 'void **' underneath */
};

/*--- Functions prototypes ---*/
void recvHandler(struct UartData * uartData);
