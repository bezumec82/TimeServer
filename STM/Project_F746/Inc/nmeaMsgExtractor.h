#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "cmsis_os.h"

#include "main.h"

/* Send directly to output w/out extraction */
#define TEST_RUN				false

/* Output to UART extracted packages */
#define DEBUG_EXTRACTOR			true

#define PARSER_SEMA_TIMEOUT		20

/*--- Parsing related ---*/
#define NMEA_START_CHAR			'$'
/* NMEA message end sequence is 0x0d->0x0A*/
#define NMEA_MSG_END_0			0x0d
#define NMEA_MSG_END_1			0x0a

/*--- Memory related ---*/
#define NMEA_MSG_LIST_LEN		8
#define NMEA_MSG_MAX_SIZE		128

struct ExtrrState
{
	bool 	startFound; 	//!< Flag shows that symbol '$' was found in 'src' data
	uint8_t lastSrcSymbol;	//!< Last symbol of processed 'src' data
};

void nmeaMsgExtractor(	uint8_t * src, size_t srcSize,
						struct Node ** dst_ptr,
						struct ExtrrState * state);
