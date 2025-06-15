################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Core/Src/sensors/ubloxm9n_rb/ubloxm9n_rb.cc 

CC_DEPS += \
./Core/Src/sensors/ubloxm9n_rb/ubloxm9n_rb.d 

OBJS += \
./Core/Src/sensors/ubloxm9n_rb/ubloxm9n_rb.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/sensors/ubloxm9n_rb/%.o Core/Src/sensors/ubloxm9n_rb/%.su Core/Src/sensors/ubloxm9n_rb/%.cyclo: ../Core/Src/sensors/ubloxm9n_rb/%.cc Core/Src/sensors/ubloxm9n_rb/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_PWR_LDO_SUPPLY -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -Wno-address-of-packed-member -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-sensors-2f-ubloxm9n_rb

clean-Core-2f-Src-2f-sensors-2f-ubloxm9n_rb:
	-$(RM) ./Core/Src/sensors/ubloxm9n_rb/ubloxm9n_rb.cyclo ./Core/Src/sensors/ubloxm9n_rb/ubloxm9n_rb.d ./Core/Src/sensors/ubloxm9n_rb/ubloxm9n_rb.o ./Core/Src/sensors/ubloxm9n_rb/ubloxm9n_rb.su

.PHONY: clean-Core-2f-Src-2f-sensors-2f-ubloxm9n_rb

