#include <NmeaReceiver.h>
#include "irq.h"

extern NmeaReceiver pcNmeaRecv;
extern NmeaReceiver gpsNmeaRecv;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == pcNmeaRecv.getUart())
	{
		osSemaphoreRelease(pcNmeaRecv.getSema());
	}
	if(huart == gpsNmeaRecv.getUart())
	{
		osSemaphoreRelease(gpsNmeaRecv.getSema());
	}

}

extern bool debugUart_isSending;
extern UART_HandleTypeDef * debugUart;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == debugUart)
	{
		debugUart_isSending = false;
	}
}

/* USER CODE END 1 */
