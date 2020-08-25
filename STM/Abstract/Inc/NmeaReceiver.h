#pragma once

#ifdef STM32H757xx
# include "stm32h7xx_hal.h"
#endif

#ifdef STM32F746xx
# include "stm32f7xx_hal.h"
#endif

#include "cmsis_os.h"

#include "nmeaMsgExtractor.h"
#include "DLList.h"

/* Common part - out of abstraction */
#define UART_RX_TIMEOUT_MS 	20
#define UART_MAX_DELAY_MS	1000
#define UART_RX_BUF_SIZE    64

class NmeaReceiver
{

public :
	NmeaReceiver() = default;
	int initialize(UART_HandleTypeDef * uart_handle);
	~NmeaReceiver() = default;

public : /*--- Getters, setters ---*/
	osSemaphoreId_t& getSema()
	{
		return rxSema;
	}
	UART_HandleTypeDef * getUart()
	{
		return uartHandle;
	}
private :
	static void threadFunc(void * argument);


private : /*--- Properties ---*/
	UART_HandleTypeDef * uartHandle = nullptr;
	struct Node * msgList = nullptr;
	osThreadId_t thrdId = nullptr;
	const osThreadAttr_t thrdAttr =
	{
		.name = "RX thread"
	};
	osSemaphoreId_t rxSema = nullptr;
	const osSemaphoreAttr_t semaAttr =
	{
		.name = "UART RX semaphore"
	};

};

