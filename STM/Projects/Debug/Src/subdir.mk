################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/NMEA_parser.c \
../Src/circBuf.c \
../Src/freertos.c \
../Src/main.c \
../Src/stm32f7xx_hal_msp.c \
../Src/stm32f7xx_hal_timebase_tim.c \
../Src/stm32f7xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f7xx.c \
../Src/uart.c \
../Src/usb_device.c \
../Src/usbd_cdc_if.c \
../Src/usbd_conf.c \
../Src/usbd_desc.c 

OBJS += \
./Src/NMEA_parser.o \
./Src/circBuf.o \
./Src/freertos.o \
./Src/main.o \
./Src/stm32f7xx_hal_msp.o \
./Src/stm32f7xx_hal_timebase_tim.o \
./Src/stm32f7xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f7xx.o \
./Src/uart.o \
./Src/usb_device.o \
./Src/usbd_cdc_if.o \
./Src/usbd_conf.o \
./Src/usbd_desc.o 

C_DEPS += \
./Src/NMEA_parser.d \
./Src/circBuf.d \
./Src/freertos.d \
./Src/main.d \
./Src/stm32f7xx_hal_msp.d \
./Src/stm32f7xx_hal_timebase_tim.d \
./Src/stm32f7xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f7xx.d \
./Src/uart.d \
./Src/usb_device.d \
./Src/usbd_cdc_if.d \
./Src/usbd_conf.d \
./Src/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -DUSE_HAL_DRIVER -DSTM32F746xx -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Inc" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Drivers/STM32F7xx_HAL_Driver/Inc" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"/media/constantine/Work/LEMZ/TimeServer/STM/Projects/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


