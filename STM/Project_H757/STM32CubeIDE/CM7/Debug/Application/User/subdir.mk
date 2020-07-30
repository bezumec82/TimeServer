################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/media/constantine/Work/LEMZ/TimeServer/STM/Project_H757/CM7/Src/freertos.c \
/media/constantine/Work/LEMZ/TimeServer/STM/Project_H757/CM7/Src/main.c \
/media/constantine/Work/LEMZ/TimeServer/STM/Project_H757/CM7/Src/stm32h7xx_hal_msp.c \
/media/constantine/Work/LEMZ/TimeServer/STM/Project_H757/CM7/Src/stm32h7xx_hal_timebase_tim.c \
/media/constantine/Work/LEMZ/TimeServer/STM/Project_H757/CM7/Src/stm32h7xx_it.c \
../Application/User/syscalls.c \
../Application/User/sysmem.c 

C_DEPS += \
./Application/User/freertos.d \
./Application/User/main.d \
./Application/User/stm32h7xx_hal_msp.d \
./Application/User/stm32h7xx_hal_timebase_tim.d \
./Application/User/stm32h7xx_it.d \
./Application/User/syscalls.d \
./Application/User/sysmem.d 

OBJS += \
./Application/User/freertos.o \
./Application/User/main.o \
./Application/User/stm32h7xx_hal_msp.o \
./Application/User/stm32h7xx_hal_timebase_tim.o \
./Application/User/stm32h7xx_it.o \
./Application/User/syscalls.o \
./Application/User/sysmem.o 


# Each subdirectory must supply rules for building sources it contributes
Application/User/freertos.o: /media/constantine/Work/LEMZ/TimeServer/STM/Project_H757/CM7/Src/freertos.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H757xx -DCORE_CM7 -DDEBUG -c -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/CMSIS/Include -I../../../CM7/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I"/media/constantine/Work/LEMZ/TimeServer/STM/Abstract/Inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/freertos.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/main.o: /media/constantine/Work/LEMZ/TimeServer/STM/Project_H757/CM7/Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H757xx -DCORE_CM7 -DDEBUG -c -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/CMSIS/Include -I../../../CM7/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I"/media/constantine/Work/LEMZ/TimeServer/STM/Abstract/Inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32h7xx_hal_msp.o: /media/constantine/Work/LEMZ/TimeServer/STM/Project_H757/CM7/Src/stm32h7xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H757xx -DCORE_CM7 -DDEBUG -c -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/CMSIS/Include -I../../../CM7/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I"/media/constantine/Work/LEMZ/TimeServer/STM/Abstract/Inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/stm32h7xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32h7xx_hal_timebase_tim.o: /media/constantine/Work/LEMZ/TimeServer/STM/Project_H757/CM7/Src/stm32h7xx_hal_timebase_tim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H757xx -DCORE_CM7 -DDEBUG -c -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/CMSIS/Include -I../../../CM7/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I"/media/constantine/Work/LEMZ/TimeServer/STM/Abstract/Inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/stm32h7xx_hal_timebase_tim.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32h7xx_it.o: /media/constantine/Work/LEMZ/TimeServer/STM/Project_H757/CM7/Src/stm32h7xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H757xx -DCORE_CM7 -DDEBUG -c -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/CMSIS/Include -I../../../CM7/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I"/media/constantine/Work/LEMZ/TimeServer/STM/Abstract/Inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/stm32h7xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/syscalls.o: ../Application/User/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H757xx -DCORE_CM7 -DDEBUG -c -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/CMSIS/Include -I../../../CM7/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I"/media/constantine/Work/LEMZ/TimeServer/STM/Abstract/Inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/sysmem.o: ../Application/User/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H757xx -DCORE_CM7 -DDEBUG -c -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/CMSIS/Include -I../../../CM7/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I"/media/constantine/Work/LEMZ/TimeServer/STM/Abstract/Inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

