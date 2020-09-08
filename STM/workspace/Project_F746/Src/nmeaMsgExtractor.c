#include "nmeaMsgExtractor.h"

/*!
 * @param src Data buffer to take NMEA packages from.
 * @param srcSize Accessible size of data buffer.
 * @param dst_ptr Pointer to the list of messages.
 * @note Each time message have found, next node is taken.
 * @param state State of extractor.
 */
void nmeaMsgExtractor(	uint8_t * src, size_t srcSize,
					struct Node ** dst_ptr,
					struct ExtrrState * state)
{
	uint32_t srcIdx = 0;
	struct Node * dst = *dst_ptr;
#if DEBUG_EXTRACTOR
#endif
	for( ;srcIdx < srcSize; srcIdx++)
	{
		if( (state->startFound) && (src[srcIdx] == NMEA_START_CHAR) )
		{
			debug("Broken message. Second start.\r\n");
#if DEBUG_EXTRACTOR //Gives clear riding of buffer
			memset(&dst->buf[0], 0x00, dst->bufSize);
#endif
			dst->bufIdx = 0;
		}

		if( (!state->startFound) && (src[srcIdx] == NMEA_START_CHAR) )
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
				extern UART_HandleTypeDef * debugUart;
				extern bool debugUart_isSending;
				if(!debugUart_isSending) //unblock from IRQ
				{
					debugUart_isSending = true;
					HAL_UART_Transmit_DMA(debugUart, \
							&dst->buf[0], dst->bufIdx);
				}
#endif
				state->startFound = false; //full message found
				* dst_ptr = dst->next; //move to the next buffer
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
