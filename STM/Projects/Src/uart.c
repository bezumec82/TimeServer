#include "uart.h"

/*!
 * @brief Data reception mechanism.
 * @note Should handled inside 'for(;;)' in thread for
 * 		continuous data handling.
 * @param uartData Structure that holds all relevant information.
 */
void recvHandler(struct UartData * uartData)
{
	uint8_t rxBuf[UART_RX_CHUNK + 1] = {}; //1 protection byte
	if(HAL_UART_Receive_DMA(uartData->uartHandle, rxBuf, UART_RX_CHUNK)!= HAL_OK)
	{
		printf("Reception error.\r\n");
	}
	/* Try to obtain the RX semaphore from DMA IRQ */
	osStatus_t status = osSemaphoreAcquire(* uartData->rxSemaHandle , \
			UART_RX_TIMEOUT_MS/portTICK_PERIOD_MS);
	switch(status)
	{
		case osOK :
		{
			circBufPut(uartData->cBuf, rxBuf, UART_RX_CHUNK);
			osSemaphoreRelease(* uartData->parseSemaHandle); //pass ball to the parser
			break;
		}
		case osErrorTimeout :
		{
			HAL_UART_AbortReceive_IT(uartData->uartHandle);
			//get data from buffer
			int idx = 0;
			for( ; rxBuf[idx] != 0x00; idx++)
			{}
			if(idx) //if data in buffer pass it to the parser
			{
				circBufPut(uartData->cBuf, rxBuf, idx);
				osSemaphoreRelease(* uartData->parseSemaHandle);
			}
			break;
		}
		default :
		{
			printf("Unhandled error.\r\n");
			break;
		}
	} //end switch
	//check protection byte in place
	assert((rxBuf[UART_RX_CHUNK] == 0x00) && \
			"UART RX buf overflow");
}
