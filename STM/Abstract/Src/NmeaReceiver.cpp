#include "NmeaReceiver.h"

int NmeaReceiver::initialize(UART_HandleTypeDef * uart_handle)
{
	uartHandle = uart_handle;

	msgList = initList(NMEA_MSG_MAX_SIZE);
	if(!msgList) return EXIT_FAILURE;
	for(int bufNum = 1 /*exclude head node*/; \
		bufNum < NMEA_MSG_LIST_LEN; bufNum++)
	{
		addNode(msgList);
	}

	rxSema = osSemaphoreNew(1, 0, &semaAttr);
	thrdId = osThreadNew(
			&NmeaReceiver::threadFunc, this, &thrdAttr);
	return EXIT_SUCCESS;
};


#if(0)
NmeaReceiver::~NmeaReceiver()
{
	osThreadTerminate(thrdId);
	osSemaphoreDelete(rxSema);
};
#endif

void NmeaReceiver::threadFunc(void * argument)
{
	/* All local variables will end in the stack of function.
	 * Each thread has its own stack. */
	uint32_t semaAcqAtmptCnt = 0;

	/* Initialization */
	uint32_t rxBufIdx = 0;
	uint32_t rxBufSize = UART_RX_BUF_SIZE;
	uint8_t rxBuf[rxBufSize + 1]; //1 protection byte
	rxBuf[rxBufSize] = MEM_PROT_SYMBOL;

	struct ExtrrState extrrState =
	{
		.startFound = false,
		.lastSrcSymbol = 0x00
	};

	if(argument == nullptr)
	{
		printf("Null passed as the parameter.\r\n");
		HAL_NVIC_SystemReset();
	}
	NmeaReceiver * instance = static_cast<NmeaReceiver*>(argument);

	/* Ignite transmission cycle */
reignite:
	debug("Ignition.\r\n");
	semaAcqAtmptCnt = 0;
	rxBufIdx = 0;
	memset(rxBuf, 0x00, UART_RX_BUF_SIZE); //clear
	HAL_UART_Receive_DMA(instance->uartHandle,
		&rxBuf[0], rxBufSize);

	/* Infinite loop */
	for(;;)
	{
		/* Try to obtain the RX semaphore from DMA IRQ */
		osStatus_t semaStatus = osSemaphoreAcquire(instance->rxSema,
				UART_RX_TIMEOUT_MS/portTICK_PERIOD_MS);
		switch(semaStatus)
		{
			case osErrorTimeout : //get what you can at the moment
			{
				semaAcqAtmptCnt++;
				if(semaAcqAtmptCnt ==
					UART_MAX_DELAY_MS/UART_RX_TIMEOUT_MS)
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
						&instance->msgList, &extrrState);
				}
				break;
			} //end case timeout
			case osOK : //semaphore acquired
			{
				nmeaMsgExtractor(&rxBuf[rxBufIdx],
						rxBufSize - rxBufIdx,
							&instance->msgList, &extrrState);
				//next cycle
				rxBufIdx = 0;
				memset(rxBuf, 0x00, rxBufSize); //clear
				semaAcqAtmptCnt = 0;
				HAL_UART_Receive_DMA(instance->uartHandle,
					rxBuf, rxBufSize);
				break;
			} //end case ok
			default :
			{
				debug("Unhandled error.\r\n");
				break;
			}
		} //end switch sema status
		checkProtection(instance->msgList);
		assert( (rxBuf[rxBufSize] == MEM_PROT_SYMBOL) && \
						"RX buffer overflow");
	} //end for
	//never achieved here
	HAL_NVIC_SystemReset();
}



