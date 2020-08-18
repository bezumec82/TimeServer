#include "AbstractMain.h"
#include "NmeaReceiver.h"
#include "echoTest.h"

/*--- Global variables ---*/
struct Node * gpsNmea_msgList;
struct Node * pcNmea_msgList;

/* Flags */
//To prevent send disruption - not fatal but annoying
bool debugUart_isSending = false;

/* Handles */
/* Tie to the hardware */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart8;

UART_HandleTypeDef * debugUart	= &huart1;

NmeaReceiver pcNmeaRecv;
NmeaReceiver gpsNmeaRecv;

#ifdef __cplusplus
extern "C" {
#endif


/*!
 * This function should be run after 'osKernelInitialize'.
 * Best place is right before call to the 'osKernelStart'.
 * @return
 */
int _main(void)
{
#if(ECHO_TEST)
	echoTest();
#else
	if(pcNmeaRecv.initialize(&huart8) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if(gpsNmeaRecv.initialize(&huart6) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
#endif
	printf("Custom part successfully initialized.\r\n");
  	return EXIT_SUCCESS;
}



void vApplicationMallocFailedHook( void )
{
    printf( "Allocation failed.\r\n" );
}

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
