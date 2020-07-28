#pragma once

#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"

#define TEST_RUN				true
#define PARSER_SEMA_TIMEOUT		20

#define NMEA_START_CHAR			'$'
#define NMEA_MIN_MSG_SIZE		6



/*--- External global variables ---*/
extern osThreadId_t NMEA_parser_thrdHandle;
extern const osThreadAttr_t NMEA_parser_thrd_attributes;

extern osSemaphoreId_t NMEA_GPS_parser_semaHandle;
extern const osSemaphoreAttr_t NMEA_GPS_parser_sema_attributes;

extern osSemaphoreId_t NMEA_PC_parser_semaHandle;
extern const osSemaphoreAttr_t NMEA_PC_parser_sema_attributes;

/*--- Export functions ---*/
void NMEA_parser(void *argument);
