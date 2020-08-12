#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef STM32H757xx
# include "stm32h7xx_hal.h"
#endif

#ifdef STM32F746xx
# include "stm32f7xx_hal.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define MEM_PROT_SYMBOL		0xAA //used in all buffers

#ifndef POSIX
#define MALLOC 		pvPortMalloc
#define FREE 		vPortFree
#endif

#define ECHO_TEST 	true

int _main(void);

#ifdef __cplusplus
}
#endif
