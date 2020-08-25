#include "debug.hpp"

extern UART_HandleTypeDef huart1;

UART_HandleTypeDef * debugUart = &huart1;

void debug(const char * msg)
{
	HAL_UART_Transmit(debugUart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  Retargets the C library printf function to the USART.
  */
int _write(int fd, char * ptr, int len)
{
	HAL_UART_Transmit(debugUart, (uint8_t *)ptr, len, HAL_MAX_DELAY);
	return len;
}

#ifdef __cplusplus
}
#endif
