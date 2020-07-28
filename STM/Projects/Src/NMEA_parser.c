#include "NMEA_parser.h"

/*--- Global data ---*/
osThreadId_t NMEA_parser_thrdHandle;
const osThreadAttr_t NMEA_parser_thrd_attributes = {
  .name = "NMEA_parser_thrd",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};

osSemaphoreId_t NMEA_GPS_parser_semaHandle;
const osSemaphoreAttr_t NMEA_GPS_parser_sema_attributes = {
  .name = "NMEA from GPS recieved"
};

osSemaphoreId_t NMEA_PC_parser_semaHandle;
const osSemaphoreAttr_t NMEA_PC_parser_sema_attributes = {
  .name = "NMEA from PC received"
};


extern struct CircBuf cBufGPSrecv;
extern UART_HandleTypeDef * uartForPC;

/* When data is in buffer, semaphore will be given */
void NMEA_parser(void *argument)
{
	for(;;)
	{
		/* Try to obtain the semaphore */
		osStatus_t status = osSemaphoreAcquire(NMEA_GPS_parser_semaHandle , \
				PARSER_SEMA_TIMEOUT/portTICK_PERIOD_MS);
		size_t size = availableData(&cBufGPSrecv);
		switch(status)
		{
			case osErrorTimeout :
			{
				if(size == 0)
					break;
				//else falldown
			}
			case osOK :
			{
				uint8_t * data = pvPortMalloc(size);
				circBufGet(&cBufGPSrecv, data, size);
				if(HAL_UART_Transmit_DMA(uartForPC, data, size)!= HAL_OK)
				{
					Error_Handler();
				}
				vPortFree(data);
				break;
			} //end case osOK

			default :
			{
				printf("Unhandled error.\r\n");
				break;
			}
		}
		osDelay(1);
	}
}
