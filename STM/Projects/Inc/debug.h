#pragma once

#include <stdint.h>
#include <string.h>
#include "stm32f7xx_hal.h"

extern UART_HandleTypeDef * uartForPC;
extern UART_HandleTypeDef * uartForGPS;
extern UART_HandleTypeDef * uartPrintf;

#define UART_TO_USE		uartPrintf

void debug(char * msg);
