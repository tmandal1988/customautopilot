################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Core/Archive/Src/pubsub/topic.cc 

CC_DEPS += \
./Core/Archive/Src/pubsub/topic.d 

OBJS += \
./Core/Archive/Src/pubsub/topic.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Archive/Src/pubsub/%.o Core/Archive/Src/pubsub/%.su Core/Archive/Src/pubsub/%.cyclo: ../Core/Archive/Src/pubsub/%.cc Core/Archive/Src/pubsub/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Archive-2f-Src-2f-pubsub

clean-Core-2f-Archive-2f-Src-2f-pubsub:
	-$(RM) ./Core/Archive/Src/pubsub/topic.cyclo ./Core/Archive/Src/pubsub/topic.d ./Core/Archive/Src/pubsub/topic.o ./Core/Archive/Src/pubsub/topic.su

.PHONY: clean-Core-2f-Archive-2f-Src-2f-pubsub

