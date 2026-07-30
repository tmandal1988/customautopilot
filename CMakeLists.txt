cmake_minimum_required(VERSION 3.22)

project(customAutopilotBoard C CXX ASM)

option(IST8310_ENABLE_DIAGNOSTICS
       "Compile IST8310 USB debug/error output and its print task" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(MCU_FLAGS
    -mcpu=cortex-m7
    -mfpu=fpv5-d16
    -mfloat-abi=hard
    -mthumb
)

set(PROJECT_DEFINES
    DEBUG
    USE_HAL_DRIVER
    STM32H743xx
    USE_PWR_LDO_SUPPLY
)

# --- Auto-discover header directories under Core/Inc and Core/Src -----------
file(GLOB_RECURSE ALL_HEADERS CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/Core/Inc/*.h
    ${CMAKE_SOURCE_DIR}/Core/Inc/*.hpp
    ${CMAKE_SOURCE_DIR}/Core/Src/*.h
    ${CMAKE_SOURCE_DIR}/Core/Src/*.hpp
)
list(FILTER ALL_HEADERS EXCLUDE REGEX "/Core/Archive/")
list(FILTER ALL_HEADERS EXCLUDE REGEX "/Core/Src/rc_inputs/")
set(APP_INCLUDE_DIRS "")
foreach(h ${ALL_HEADERS})
    get_filename_component(d ${h} DIRECTORY)
    list(APPEND APP_INCLUDE_DIRS ${d})
endforeach()
list(REMOVE_DUPLICATES APP_INCLUDE_DIRS)

# --- Third-party / fixed include paths --------------------------------------
set(LIB_INCLUDES
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${CMAKE_SOURCE_DIR}/USB_DEVICE/App
    ${CMAKE_SOURCE_DIR}/USB_DEVICE/Target
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
    ${CMAKE_SOURCE_DIR}/FATFS/Target
    ${CMAKE_SOURCE_DIR}/FATFS/App
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src
)

# --- Application sources: auto-discovered under Core/Src --------------------
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/Core/Src/*.c
    ${CMAKE_SOURCE_DIR}/Core/Src/*.cc
    ${CMAKE_SOURCE_DIR}/Core/Src/*.cpp
)
list(FILTER APP_SOURCES EXCLUDE REGEX "/Core/Archive/")
list(FILTER APP_SOURCES EXCLUDE REGEX "/Core/Src/rc_inputs/")

# --- Library/middleware sources: explicit (only what the build uses) --------
set(LIB_SOURCES
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c
    FATFS/App/fatfs.c
    FATFS/Target/bsp_driver_sd.c
    FATFS/Target/fatfs_platform.c
    FATFS/Target/sd_diskio.c
    Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    Middlewares/Third_Party/FatFs/src/diskio.c
    Middlewares/Third_Party/FatFs/src/ff.c
    Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
    Middlewares/Third_Party/FatFs/src/option/ccsbcs.c
    Middlewares/Third_Party/FatFs/src/option/syscall.c
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    Middlewares/Third_Party/FreeRTOS/Source/list.c
    Middlewares/Third_Party/FreeRTOS/Source/queue.c
    Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    Middlewares/Third_Party/FreeRTOS/Source/timers.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    USB_DEVICE/App/usb_device.c
    USB_DEVICE/App/usbd_cdc_if.c
    USB_DEVICE/App/usbd_desc.c
    USB_DEVICE/Target/usbd_conf.c
)

set(ASM_SOURCES
    Core/Startup/startup_stm32h743zitx.s
)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32H743ZITX_FLASH.ld)

add_executable(${PROJECT_NAME}.elf ${APP_SOURCES} ${LIB_SOURCES} ${ASM_SOURCES})

target_include_directories(${PROJECT_NAME}.elf PRIVATE ${APP_INCLUDE_DIRS} ${LIB_INCLUDES})
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE ${PROJECT_DEFINES})
if(IST8310_ENABLE_DIAGNOSTICS)
    target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
        IST8310_ENABLE_DIAGNOSTICS=1)
endif()

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    ${MCU_FLAGS}
    -ffunction-sections
    -fdata-sections
    -Wall
    -fstack-usage
    -g3
    --specs=nano.specs
    $<$<COMPILE_LANGUAGE:C>:-O0>
    $<$<COMPILE_LANGUAGE:CXX>:-Ofast>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-address-of-packed-member>
    $<$<COMPILE_LANGUAGE:ASM>:-x>
    $<$<COMPILE_LANGUAGE:ASM>:assembler-with-cpp>
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    ${MCU_FLAGS}
    -T${LINKER_SCRIPT}
    --specs=nosys.specs
    --specs=nano.specs
    -static
    -u _printf_float
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
)

set(FIRMWARE_OUTPUT_DIR "${CMAKE_BINARY_DIR}")
set(FIRMWARE_BIN_FILE "${FIRMWARE_OUTPUT_DIR}/${PROJECT_NAME}.bin")
set(FIRMWARE_HEX_FILE "${FIRMWARE_OUTPUT_DIR}/${PROJECT_NAME}.hex")

set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    LINK_DEPENDS ${LINKER_SCRIPT}
    RUNTIME_OUTPUT_DIRECTORY "${FIRMWARE_OUTPUT_DIR}"
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    BYPRODUCTS
        "${FIRMWARE_BIN_FILE}"
        "${FIRMWARE_HEX_FILE}"
    COMMAND "${CMAKE_SIZE}" "$<TARGET_FILE:${PROJECT_NAME}.elf>"
    COMMAND "${CMAKE_OBJCOPY}" -O binary "$<TARGET_FILE:${PROJECT_NAME}.elf>" "${FIRMWARE_BIN_FILE}"
    COMMAND "${CMAKE_OBJCOPY}" -O ihex   "$<TARGET_FILE:${PROJECT_NAME}.elf>" "${FIRMWARE_HEX_FILE}"
    COMMENT "Generating size report, .bin and .hex"
    VERBATIM
)
