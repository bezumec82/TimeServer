################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -DUSE_HAL_DRIVER -DSTM32F746xx -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Inc" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Drivers/STM32F7xx_HAL_Driver/Inc" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


