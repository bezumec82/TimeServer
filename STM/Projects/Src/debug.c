#include "debug.h"

extern bool debugUart_isSending;
extern UART_HandleTypeDef * debugUart;

void debug(char * msg)
{
	if(debugUart_isSending)
		return; //just drop the message
	HAL_UART_Transmit_DMA(debugUart, (uint8_t *)msg, strlen(msg));
	debugUart_isSending = true;
}
