#pragma once

#include <stdint.h>

struct IWDG_REGS /* Taken directly from STM32 HAL */
{
  uint32_t KR;   /*!< IWDG Key register,       Address offset: 0x00 */
  uint32_t PR;   /*!< IWDG Prescaler register, Address offset: 0x04 */
  uint32_t RLR;  /*!< IWDG Reload register,    Address offset: 0x08 */
  uint32_t SR;   /*!< IWDG Status register,    Address offset: 0x0C */
  uint32_t WINR; /*!< IWDG Window register,    Address offset: 0x10 */
};
