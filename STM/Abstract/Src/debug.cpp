#include "debug.h"

extern bool debugUart_isSending;
extern UART_HandleTypeDef * debugUart;

void debug(const char * msg)
{

	if(debugUart_isSending) return; //just drop the message
	debugUart_isSending = true;

	HAL_UART_Transmit_DMA(debugUart, (uint8_t *)msg, strlen(msg));

}
