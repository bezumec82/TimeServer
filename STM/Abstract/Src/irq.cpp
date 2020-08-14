#include <NmeaReceiver.h>
#include "irq.h"

extern NmeaReceiver pcNmeaRecv;
extern NmeaReceiver gpsNmeaRecv;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#if(ECHO_TEST)
	extern UART_HandleTypeDef * testUart;
	extern osSemaphoreId_t echoSema;
	if(huart == testUart)
	{
		osSemaphoreRelease(echoSema);
	}
#else
	if(huart == pcNmeaRecv.getUart())
	{
		osSemaphoreRelease(pcNmeaRecv.getSema());
	}
	if(huart == gpsNmeaRecv.getUart())
	{
		osSemaphoreRelease(gpsNmeaRecv.getSema());
	}
#endif
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

/*!
 * Stupid CubeMX forget to generate this handler from time to time.
 * Pre-process out this part in case of compile errors.
 */
#if(1)
void TIM1_UP_IRQHandler(void)
{
	extern TIM_HandleTypeDef htim1;
 	HAL_TIM_IRQHandler(&htim1);
}
#endif

