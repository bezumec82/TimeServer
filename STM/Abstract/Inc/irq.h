#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_it.h"

#include "FreeRTOS.h"
#include "task.h"

#include "AbstractMain.h"

/*--- Function prototypes ---*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);

void TIM1_UP_IRQHandler(void);

#ifdef __cplusplus
}
#endif
