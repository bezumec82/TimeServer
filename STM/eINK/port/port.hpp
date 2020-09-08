#pragma once

#include "../Src/eINK.hpp"

#if defined STM32H757xx || defined STM32H755xx
# include "stm32h7xx_hal.h"
#endif

#ifdef STM32F746xx
# include "stm32f7xx_hal.h"
#endif

/* GPIO (PORT, PIN - look CubeMX) */
#define eINK_RST_port   GPIOF
#define eINK_RST_pin    GPIO_PIN_10

#define eINK_DC_port    GPIOF
#define eINK_DC_pin     GPIO_PIN_6

#define eINK_CS_port    GPIOE
#define eINK_CS_pin     GPIO_PIN_11

#define eINK_BUSY_port  GPIOG
#define eINK_BUSY_pin   GPIO_PIN_6

#define eINK_RST        eINK_RST_port,  eINK_RST_pin
#define eINK_DC         eINK_DC_port,   eINK_DC_pin
#define eINK_CS         eINK_CS_port,   eINK_CS_pin
#define eINK_BUSY       eINK_BUSY_port, eINK_BUSY_pin

extern SPI_HandleTypeDef * spiHandle;

/* GPIO read and write */
#define GPIO_WRITE(_pin, _value)    \
        HAL_GPIO_WritePin(_pin, _value == 0? GPIO_PIN_RESET:GPIO_PIN_SET)
#define GPIO_READ(_pin)             HAL_GPIO_ReadPin(_pin)

namespace eINK
{

    template<typename Uint, typename >
    void IL0373::yield(Uint&& ms)
    {
        HAL_Delay(::std::forward<Uint>(ms));
        //Yield(); /* LaOS */
        //vTaskDelay(::std::forward<Uint>(ms));
        //osDelay(::std::forward<Uint>(ms));
    }


    template < typename Ptr, typename >
    void IL0373::writeByte(Ptr&& ptr)
    {
        HAL_SPI_Transmit(spiHandle, ::std::forward<Ptr>(ptr),
                1, HAL_MAX_DELAY);
    }

} //end namespace
