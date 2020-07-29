#include "NMEAmsgExtractor.h"

/*--- Global data ---*/
osThreadId_t pcNMEAmsgExtrr_thrdHandle;
const osThreadAttr_t pcNMEAmsgExtrr_thrdAtts = {
  .name = "pcNMEAmsgExtrr_thrd",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};

osThreadId_t gpsNMEAmsgExtrr_thrdHandle;
const osThreadAttr_t gpsNMEAmsgExtrr_thrdAtts = {
  .name = "pcNMEAmsgExtrr_thrd",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};

osSemaphoreId_t gpsNMEAmsgExtrr_semaHandle;
const osSemaphoreAttr_t gpsNMEAmsgExtrr_semaAttrs = {
  .name = "NMEA from GPS received"
};

osSemaphoreId_t pcNMEAmsgExtrr_semaHandle;
const osSemaphoreAttr_t pcNMEAmsgExtrr_semaAttrs = {
  .name = "NMEA from PC received"
};

extern struct CircBuf cBufGPSrecv;

/*!
 * @brief This data should pertain across different
 * data chunks received from UART.
 */
struct ExtrrState
{
	bool 	startFound; 	//!< Flag shows that symbol '$' was found in 'src' data
	uint8_t lastSrcSymbol;	//!< Last symbol of processed 'src' data
};

/*!
 * @param src Data buffer to take NMEA packages from.
 * @param srcSize Accessible size of data buffer.
 * @param dst_ptr Pointer to the list of messages.
 * @note Each time message have found, next node is taken.
 * @param state State of extractor.
 */
void msgExtractor(	uint8_t * src, size_t srcSize,
					struct Node ** dst_ptr,
					struct ExtrrState * state)
{
	uint32_t srcIdx = 0;
	struct Node * dst = *dst_ptr;
#if DEBUG_EXTRACTOR
#endif
	for( ;srcIdx < srcSize; srcIdx++)
	{
		if( (state->startFound) && (src[srcIdx] == '$') )
		{
			debug("Broken message. Second start.\r\n");
#if DEBUG_EXTRACTOR //Gives clear riding of buffer
			memset(&dst->buf[0], 0x00, dst->bufSize);
#endif
			dst->bufIdx = 0;
		}

		if( (!state->startFound) && (src[srcIdx] == '$') )
		{
			state->startFound = true;
		}

		if(state->startFound)
		{
			dst->buf[dst->bufIdx] = src[srcIdx];
			dst->bufIdx++;
			if(dst->bufIdx == dst->bufSize)
			{
				debug("Broken message. Too big.\r\n");
				dst->bufIdx = 0;
				state->startFound = false;
			}
			//look for message end
			if( (src[srcIdx] == NMEA_MSG_END_1) && \
					(state->lastSrcSymbol == NMEA_MSG_END_0) )
			{
#if DEBUG_EXTRACTOR //Output to the UART
				extern UART_HandleTypeDef * uartForPC;
				extern bool uartForPC_isSending;
				if(!uartForPC_isSending) //unblock from IRQ
				{
					uartForPC_isSending = true;
					HAL_StatusTypeDef uartStatus = \
							HAL_UART_Transmit_DMA(uartForPC, \
									&dst->buf[0], dst->bufIdx);
					if(uartStatus!= HAL_OK)
					{
						debug("UART transmit error.\r\n");
					}
				}
#endif
				//full message found
				state->startFound = false;
				//move to the next buffer
				* dst_ptr = dst->next;
				dst = * dst_ptr;

				dst->bufIdx = 0;
#if DEBUG_EXTRACTOR
				memset(&dst->buf[0], 0x00, dst->bufSize);
#endif
			}
			state->lastSrcSymbol = src[srcIdx];
		}
	} //end for
}

/* When data is in circular buffer, semaphore will be given */
void gpsNMEAmsgExtractor(void *argument)
{
//	bool startFound = false;

	struct ExtrrState extrrState =
	{
		.startFound = false,
		.lastSrcSymbol = 0x00
	};

	struct Node * head = initList(NMEA_MSG_MAX_SIZE);
	int idx = 0;
	for(idx = 1 /*exclude head node*/; idx < NMEA_MSG_LIST_LEN; idx++)
	{
		addNode(head);
	}

	/* Infinite loop */
	for(;;)
	{
		/* Try to obtain the semaphore */
		osStatus_t semaStatus = osSemaphoreAcquire(gpsNMEAmsgExtrr_semaHandle , \
				PARSER_SEMA_TIMEOUT/portTICK_PERIOD_MS);
		size_t size = availableData(&cBufGPSrecv);
		switch(semaStatus)
		{
			case osErrorTimeout :
			{
				if(size == 0)
					break;
			} //fall through
			case osOK :
			{
				uint8_t * data = pvPortMalloc(size + 1); //include protector
				if(!data) break; //out of switch cycle
				data[size] = MEM_PROT_SYMBOL; //protective symbol

				circBufGet(&cBufGPSrecv, data, size);
				msgExtractor(	data , size,
								&head,
								&extrrState);
#if(0)
#if(0) //bare debug
				HAL_StatusTypeDef uartStatus = HAL_UART_Transmit_DMA(uartForPC, data, size);
				if(uartStatus!= HAL_OK)static uint8_t lastSymbol = 0x00;
				{
					printf("UART transmit error : %d.\r\n", uartStatus);
				}
#else
				/*------------------------------*/
				/*--- Extracts full messages ---*/
				/*------------------------------*/
				uint32_t dataIdx = 0;
				for( ;dataIdx < size; dataIdx++)
				{
					if( (startFound) && (data[dataIdx] == '$') )
					{
						debug("Broken message. Second start.\r\n");
						head->buf[head->bufIdx] = 0;
					}

					if( (!startFound) && (data[dataIdx] == '$') )
					{
						startFound = true;
					}

					if(startFound)
					{
						head->buf[head->bufIdx] = data[dataIdx];
						head->bufIdx++;
						if(head->bufIdx == head->bufSize)
						{
							debug("Broken message. Too big.\r\n");
							head->bufIdx = 0;
							startFound = false;
						}
						//look for message end
						static uint8_t lastSymbol = 0x00;
						if( (data[dataIdx] == NMEA_MSG_END_1) && \
								(lastSymbol == NMEA_MSG_END_0) )
						{
							/*--- Debug output---*/
							HAL_StatusTypeDef uartStatus = \
									HAL_UART_Transmit_DMA(uartForPC, &head->buf[0], head->bufIdx);
							if(uartStatus!= HAL_OK)
							{
								debug("UART transmit error.\r\n");
							}
							//full message found
							startFound = false;
							head = head->next; //get next buffer
						}
						lastSymbol = data[dataIdx];
					}
				} //end for
#endif
#endif
				assert(data[size] == MEM_PROT_SYMBOL && "Data buffer overflow");
				vPortFree(data);
				break;
			} //end case osOK

			default :
			{
				printf("Unhandled error.\r\n");
				break;
			}
		} //end switch(semaStatus)
		checkProtection(head);
	}
}
