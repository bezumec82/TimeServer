#pragma once



#include <stdio.h>
#include <stdbool.h>

#include "cmsis_os.h"

#include "main.h"

#define TEST_RUN				true
#define PARSER_SEMA_TIMEOUT		20

#define NMEA_START_CHAR			'$'
#define NMEA_MIN_MSG_SIZE		6
#define NMEA_MSG_LIST_LEN		4

/* NMEA message end sequence is 0x0d->0x0A*/
#define NMEA_MSG_END_0			0x0d
#define NMEA_MSG_END_1			0x0a

#define NMEA_MSG_MAX_SIZE		128

#define DEBUG_EXTRACTOR			true

/*--- External global variables ---*/
extern osSemaphoreId_t gpsNMEAmsgExtrr_semaHandle;
extern const osSemaphoreAttr_t gpsNMEAmsgExtrr_semaAttrs;

extern osSemaphoreId_t pcNMEAmsgExtrr_semaHandle;
extern const osSemaphoreAttr_t pcNMEAmsgExtrr_semaAttrs;

/*--- Export functions ---*/
void gpsNMEAmsgExtractor(void *argument);
