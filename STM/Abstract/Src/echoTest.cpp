#include "echoTest.h"

/* Global variables */
osThreadId_t echoThrdId = nullptr;
const osThreadAttr_t echoThrdAttr =
{
	.name = "UART echo thread"
};
osSemaphoreId_t echoSema;

/* Functions declarations */
void simpleEchoThreadFunc(void * argument);
void IRQEchoThreadFunc(void * argument);
void DMAEchoThreadFunc(void * argument);

/* Tie to the hardware */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart8;

/* UART to test */
UART_HandleTypeDef * testUart	= &huart1;
/* Type of test */
void(*threadFunc)(void*) = &DMAEchoThreadFunc;

/* Test init function */
void echoTest()
{
	echoSema = osSemaphoreNew(1, 0, NULL);
	echoThrdId = osThreadNew(
		threadFunc, nullptr, &echoThrdAttr);
}

void simpleEchoThreadFunc(void * argument)
{
	for(;;)
	{
		uint8_t buf = 0;
		HAL_UART_Receive(testUart, &buf, sizeof(buf), 1000);
		HAL_UART_Transmit(testUart, &buf, sizeof(buf), 1000);
		osDelay(1);
	}
}

void IRQEchoThreadFunc(void * argument)
{
	for(;;)
	{
		uint8_t buf = 0;
		HAL_UART_Receive_IT(testUart, &buf, sizeof(buf));
		osStatus_t semaStatus = osSemaphoreAcquire(echoSema,10);
		if(semaStatus == osOK)
		{
			HAL_UART_Transmit_IT(testUart, &buf, sizeof(buf));
		}
	}
}

#define BUF_SIZE	4
#if defined ( __CC_ARM )
ALIGN_32BYTES(

		__attribute__((section (".dma_buffer"))) uint8_t buf[BUF_SIZE]
														 ) = { };
#elif defined ( __GNUC__ )
ALIGN_32BYTES(
		__attribute__((section (".dma_buffer"))) uint8_t buf[BUF_SIZE]
														 ) = { };
#endif

void DMAEchoThreadFunc(void * argument)
{
	for(;;)
	{
#if(0)
		SCB_InvalidateDCache_by_Addr((uint32_t*)(((uint32_t)buf) &
				~(uint32_t)0x1F),
				BUF_SIZE+32);
#endif
		HAL_UART_Receive_DMA(testUart, &buf[0], sizeof(buf));
		osStatus_t semaStatus = osSemaphoreAcquire(echoSema,10);
		if(semaStatus == osOK)
		{
			HAL_UART_Transmit_DMA(testUart, &buf[0], sizeof(buf));
			osDelay(100);
			memset(&buf[0], 0x0, sizeof(buf));
		}
	}
}
