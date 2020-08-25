#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "cmsis_os.h"

#define CIRC_BUF_SIZE	1024
#define DYNAMIC_BUF		false
#define STATIC_BUF		true

#if (!(DYNAMIC_BUF ^ STATIC_BUF))
# error "Wrong config"
#endif

struct CircBuf
{
#if DYNAMIC_BUF
	uint8_t * buf;
#elif STATIC_BUF
	//One byte for the protection symbol
	uint8_t buf[CIRC_BUF_SIZE + 1];
#else
# error "Type of buffer is not set"
#endif
	uint32_t size;
	uint8_t * head;
	uint8_t * tail;
	osMutexId_t lock; //collective access of pointers in 'availableData'
};

/*--- Function prototypes ---*/
int32_t initCircBuf(struct CircBuf * cBuf, size_t size);
uint8_t * circBufPut(struct CircBuf * cBuf, uint8_t * data, size_t size);
uint32_t availableData(struct CircBuf * cBuf);
uint8_t * circBufGet(struct CircBuf * cBuf, uint8_t * data, size_t size);

#ifdef __cplusplus
}
#endif
