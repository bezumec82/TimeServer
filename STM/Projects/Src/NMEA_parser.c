#include "NMEA_parser.h"

/*--- Global data ---*/
/* Definitions for FromGPS_thrd */
osThreadId_t NMEA_parser_thrdHandle;
const osThreadAttr_t NMEA_parser_thrd_attributes = {
  .name = "NMEA_parser_thrd",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};

osSemaphoreId_t NMEA_parser_semaHandle;
const osSemaphoreAttr_t NMEA_parser_sema_attributes = {
  .name = "PC_RX_sema"
};

/* When data is in buffer, semaphore will be given */
void NMEA_parser(void *argument)
{
	for(;;)
	{
		/* Try to obtain the semaphore */
		if(osSemaphoreAcquire(NMEA_parser_semaHandle , 0) == osOK)
		{
			printf("NMEA parser sema. recvd.\n");
		}
		osDelay(1);
	}
}
