################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Core/Archive/Src/main.cc 

C_SRCS += \
../Core/Archive/Src/freertos.c \
../Core/Archive/Src/stm32h7xx_hal_msp.c \
../Core/Archive/Src/stm32h7xx_hal_timebase_tim.c \
../Core/Archive/Src/stm32h7xx_it.c \
../Core/Archive/Src/syscalls.c \
../Core/Archive/Src/sysmem.c \
../Core/Archive/Src/system_stm32h7xx.c \
../Core/Archive/Src/usb_cdc_printf.c 

C_DEPS += \
./Core/Archive/Src/freertos.d \
./Core/Archive/Src/stm32h7xx_hal_msp.d \
./Core/Archive/Src/stm32h7xx_hal_timebase_tim.d \
./Core/Archive/Src/stm32h7xx_it.d \
./Core/Archive/Src/syscalls.d \
./Core/Archive/Src/sysmem.d \
./Core/Archive/Src/system_stm32h7xx.d \
./Core/Archive/Src/usb_cdc_printf.d 

CC_DEPS += \
./Core/Archive/Src/main.d 

OBJS += \
./Core/Archive/Src/freertos.o \
./Core/Archive/Src/main.o \
./Core/Archive/Src/stm32h7xx_hal_msp.o \
./Core/Archive/Src/stm32h7xx_hal_timebase_tim.o \
./Core/Archive/Src/stm32h7xx_it.o \
./Core/Archive/Src/syscalls.o \
./Core/Archive/Src/sysmem.o \
./Core/Archive/Src/system_stm32h7xx.o \
./Core/Archive/Src/usb_cdc_printf.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Archive/Src/%.o Core/Archive/Src/%.su Core/Archive/Src/%.cyclo: ../Core/Archive/Src/%.c Core/Archive/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Archive/Src/%.o Core/Archive/Src/%.su Core/Archive/Src/%.cyclo: ../Core/Archive/Src/%.cc Core/Archive/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Archive-2f-Src

clean-Core-2f-Archive-2f-Src:
	-$(RM) ./Core/Archive/Src/freertos.cyclo ./Core/Archive/Src/freertos.d ./Core/Archive/Src/freertos.o ./Core/Archive/Src/freertos.su ./Core/Archive/Src/main.cyclo ./Core/Archive/Src/main.d ./Core/Archive/Src/main.o ./Core/Archive/Src/main.su ./Core/Archive/Src/stm32h7xx_hal_msp.cyclo ./Core/Archive/Src/stm32h7xx_hal_msp.d ./Core/Archive/Src/stm32h7xx_hal_msp.o ./Core/Archive/Src/stm32h7xx_hal_msp.su ./Core/Archive/Src/stm32h7xx_hal_timebase_tim.cyclo ./Core/Archive/Src/stm32h7xx_hal_timebase_tim.d ./Core/Archive/Src/stm32h7xx_hal_timebase_tim.o ./Core/Archive/Src/stm32h7xx_hal_timebase_tim.su ./Core/Archive/Src/stm32h7xx_it.cyclo ./Core/Archive/Src/stm32h7xx_it.d ./Core/Archive/Src/stm32h7xx_it.o ./Core/Archive/Src/stm32h7xx_it.su ./Core/Archive/Src/syscalls.cyclo ./Core/Archive/Src/syscalls.d ./Core/Archive/Src/syscalls.o ./Core/Archive/Src/syscalls.su ./Core/Archive/Src/sysmem.cyclo ./Core/Archive/Src/sysmem.d ./Core/Archive/Src/sysmem.o ./Core/Archive/Src/sysmem.su ./Core/Archive/Src/system_stm32h7xx.cyclo ./Core/Archive/Src/system_stm32h7xx.d ./Core/Archive/Src/system_stm32h7xx.o ./Core/Archive/Src/system_stm32h7xx.su ./Core/Archive/Src/usb_cdc_printf.cyclo ./Core/Archive/Src/usb_cdc_printf.d ./Core/Archive/Src/usb_cdc_printf.o ./Core/Archive/Src/usb_cdc_printf.su

.PHONY: clean-Core-2f-Archive-2f-Src

