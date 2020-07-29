#include "debug.h"

void debug(char * msg)
{
	HAL_UART_Transmit_DMA(UART_TO_USE, (uint8_t *)msg, strlen(msg));
}
