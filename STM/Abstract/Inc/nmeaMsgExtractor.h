#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifdef STM32H757xx
# include "stm32h7xx_hal.h"
#endif

#ifdef STM32F746xx
# include "stm32f7xx_hal.h"
#endif

#include "cmsis_os.h"

#include "AbstractMain.h"

#include "debug.h"
#include "DLList.h"
#include "StateMachine.h"

/* Output to UART extracted packages */
#define DEBUG_EXTRACTOR			false

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


void nmeaMsgExtractor(uint8_t * src, size_t srcSize,
						struct Node ** dst_ptr,
						struct ExtrrState * state);


#ifdef __cplusplus
}
#endif
