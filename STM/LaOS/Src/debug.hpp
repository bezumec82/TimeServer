#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef STM32H757xx
# include "stm32h7xx_hal.h"
#endif

#ifdef STM32F746xx
# include "stm32f7xx_hal.h"
#endif

/*--- Function prototypes ---*/
void debug(const char * msg);

#ifdef __cplusplus
}
#endif
