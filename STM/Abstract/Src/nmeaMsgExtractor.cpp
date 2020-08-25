#include "nmeaMsgExtractor.h"


/*!
 * @brief Simple algorithm to extract messages from
 * stream to linked list of buffers.
 * @param src Data buffer to take NMEA packages from.
 * @param srcSize Accessible size of data buffer.
 * @param dst_ptr Pointer to the list of messages.
 * @note Each time message have found, next node is taken.
 * @param state State of extractor.
 */
void nmeaMsgExtractor(uint8_t * src, size_t srcSize,
						struct Node ** dst_ptr,
						struct ExtrrState * state)
{
	uint32_t srcIdx = 0;
	struct Node * dst = *dst_ptr;
#if DEBUG_EXTRACTOR
#endif
	for( ;srcIdx < srcSize; srcIdx++)
	{
		if( (state->startFound) && (src[srcIdx]
			== NMEA_START_CHAR) )
		{
			debug("Broken message. Second start.\r\n");
#if DEBUG_EXTRACTOR //Gives clear riding of buffer
			memset((void *)&dst->buf[0], 0x00, dst->bufSize);
#endif
			dst->bufIdx = 0;
		}

		if( (!state->startFound) && (src[srcIdx]
			== NMEA_START_CHAR) )
		{
			state->startFound = true;
		}

		if(state->startFound)
		{
			dst->buf[dst->bufIdx] = src[srcIdx];
			dst->bufIdx++; //post increment -> index becomes size
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
							(uint8_t*)&dst->buf[0], dst->bufIdx);
				}

#endif
				/*------------------------*/
				/* Post to 'StateMachine' */
				/*------------------------*/
				/* Message created out of blue */
				StateMachine::Message * message =
						(StateMachine::Message *)MALLOC(sizeof(StateMachine::Message));
				message->message = &dst->buf[0];
				message->msgSize = dst->bufIdx;
				message->senderId = StateMachine::SenderId::GPS_UART;
				StateMachine::getInstance().post(message);

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
