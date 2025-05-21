################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Core/Src/rc_inputs/rc_sbus.cc 

CC_DEPS += \
./Core/Src/rc_inputs/rc_sbus.d 

OBJS += \
./Core/Src/rc_inputs/rc_sbus.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/rc_inputs/%.o Core/Src/rc_inputs/%.su Core/Src/rc_inputs/%.cyclo: ../Core/Src/rc_inputs/%.cc Core/Src/rc_inputs/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-rc_inputs

clean-Core-2f-Src-2f-rc_inputs:
	-$(RM) ./Core/Src/rc_inputs/rc_sbus.cyclo ./Core/Src/rc_inputs/rc_sbus.d ./Core/Src/rc_inputs/rc_sbus.o ./Core/Src/rc_inputs/rc_sbus.su

.PHONY: clean-Core-2f-Src-2f-rc_inputs

