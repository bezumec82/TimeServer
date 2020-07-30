#include "uart.h"

/*!
 * @brief Data reception mechanism.
 * @note There is no actual way out from this function.
 * @param uartData Structure that holds all relevant information.
 */
void uartRxThread_func(struct UartData * uartData)
{
	/* Initialization */
	uint32_t semaAcqAtmptCnt = 0;

	uint32_t rxBufIdx = 0;
	uint32_t rxBufSize = UART_RX_BUF_SIZE;
	uint8_t rxBuf[rxBufSize + 1]; //1 protection byte
	rxBuf[rxBufSize] = MEM_PROT_SYMBOL;

	struct ExtrrState extrrState =
	{
		.startFound = false,
		.lastSrcSymbol = 0x00
	};

	/* Ignite transmission cycle */
reignite:
	debug("Ignition.\r\n");
	semaAcqAtmptCnt = 0;
	rxBufIdx = 0;
	memset(rxBuf, 0x00, UART_RX_BUF_SIZE); //clear
	HAL_UART_Receive_DMA(uartData->uartHandle, &rxBuf[0], rxBufSize);

	/* Infinite loop */
	for(;;)
	{
		/* Try to obtain the RX semaphore from DMA IRQ */
		osStatus_t semaStatus = osSemaphoreAcquire(uartData->rxSemaHandle , \
				UART_RX_TIMEOUT_MS/portTICK_PERIOD_MS);
		switch(semaStatus)
		{
			case osErrorTimeout : //get what you can at the moment
			{
				semaAcqAtmptCnt++;
				if(semaAcqAtmptCnt == UART_MAX_DELAY_MS/UART_RX_TIMEOUT_MS)
				{
					goto reignite;
				}

				uint32_t avlblData = 0;
				uint32_t prevIdx = rxBufIdx;
				//define how much can be read from buffer
				for( ; (rxBuf[rxBufIdx] != 0x00) && \
					(rxBufIdx < rxBufSize);
						rxBufIdx++) //replace 'rxBufIdx' to the new terminator
				{
					avlblData++;
				}
				if(avlblData) //if data is in buffer, pass it to the parser
				{
					nmeaMsgExtractor(&rxBuf[prevIdx], avlblData /* from idx to amnt */,
						&uartData->msgList, &extrrState);
				}
				break;
			} //end case timeout
			case osOK : //semaphore acquired
			{
				nmeaMsgExtractor(&rxBuf[rxBufIdx], \
						rxBufSize - rxBufIdx, \
							&uartData->msgList, &extrrState);
				//next cycle
				rxBufIdx = 0;
				memset(rxBuf, 0x00, rxBufSize); //clear
				semaAcqAtmptCnt = 0;
				HAL_UART_Receive_DMA(uartData->uartHandle, rxBuf, rxBufSize);
				break;
			} //end case ok
			default :
			{
				debug("Unhandled error.\r\n");
				break;
			}
		} //end switch sema status
		checkProtection(uartData->msgList);
		assert( (rxBuf[rxBufSize] == MEM_PROT_SYMBOL) && \
						"RX buffer overflow");
	} //end for
	//never achieved here
}

