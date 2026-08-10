cmake_minimum_required(VERSION 3.22)

project(customAutopilotBoard C CXX ASM)

option(IST8310_ENABLE_DIAGNOSTICS
       "Compile IST8310 USB debug/error output and its print task" OFF)
option(ENABLE_RTOS_METRICS
       "Enable experimental DWT executor timing" OFF)
option(ENABLE_RTOS_METRICS_LOGGING
       "Write paced RTOS timing rows to Topic 12 (requires ENABLE_RTOS_METRICS)" OFF)
option(ENABLE_RTOS_METRICS_DEBUG_PRINT
       "Print a paced RTOS metrics summary over DEBUG_PRINT (requires ENABLE_RTOS_METRICS)" OFF)
option(ENABLE_RTOS_CONTEXT_SWITCH_METRICS
       "Count true FreeRTOS task switches (requires ENABLE_RTOS_METRICS)" OFF)
option(ENABLE_RTOS_STACK_WATERMARK_METRICS
       "Sample stack fill patterns while INACTIVE (requires metrics logging)" OFF)
option(ENABLE_RTOS_STACK_OVERFLOW_DIAGNOSTICS
       "Enable FreeRTOS level-2 stack-overflow checking for bench diagnosis" OFF)
option(ENABLE_CONTROL_PIPELINE
       "Run estimator, flight controls, and PWM in one deterministic task" OFF)
option(ENABLE_DSHOT
       "Use DShot motor output instead of analog PWM (requires ESC power cycle)" OFF)
option(ENABLE_MTF01P_DEBUG_PRINT
       "Print rate-limited MTF01P RX/data diagnostics over DEBUG_PRINT" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Parameter manifests are the single authoring source for the firmware catalog,
# typed module adapters, persistence mapping, and QGC metadata. Generate once at
# configure time so a newly added module's generated .cc file is visible to the
# source glob below, then validate/regenerate before every firmware build. The
# generator preserves mtimes when output is unchanged.
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(PARAMETER_GENERATOR
    "${CMAKE_SOURCE_DIR}/tools/parameters/generate_qgc_parameter_metadata.py")
file(GLOB_RECURSE PARAMETER_MANIFESTS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Core/Src/**/parameters.json")
execute_process(
    COMMAND "${Python3_EXECUTABLE}" "${PARAMETER_GENERATOR}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE PARAMETER_GENERATION_RESULT
    ERROR_VARIABLE PARAMETER_GENERATION_ERROR
)
if(NOT PARAMETER_GENERATION_RESULT EQUAL 0)
    message(FATAL_ERROR
        "Parameter catalog generation failed: ${PARAMETER_GENERATION_ERROR}")
endif()
file(GLOB_RECURSE PARAMETER_GENERATED_OUTPUTS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Core/Src/parameters/generated/*.h"
    "${CMAKE_SOURCE_DIR}/Core/Src/parameters/generated/*.cc"
    "${CMAKE_SOURCE_DIR}/Core/Src/parameters/generated/metadata/*.json")

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
if(ENABLE_RTOS_METRICS)
    list(APPEND PROJECT_DEFINES RTOS_METRICS_ENABLE=1)
else()
    list(APPEND PROJECT_DEFINES RTOS_METRICS_ENABLE=0)
endif()
if(ENABLE_RTOS_METRICS AND ENABLE_RTOS_METRICS_LOGGING)
    list(APPEND PROJECT_DEFINES RTOS_METRICS_LOGGING_ENABLE=1)
else()
    list(APPEND PROJECT_DEFINES RTOS_METRICS_LOGGING_ENABLE=0)
endif()
if(ENABLE_RTOS_METRICS AND ENABLE_RTOS_METRICS_DEBUG_PRINT)
    list(APPEND PROJECT_DEFINES RTOS_METRICS_DEBUG_PRINT_ENABLE=1)
else()
    list(APPEND PROJECT_DEFINES RTOS_METRICS_DEBUG_PRINT_ENABLE=0)
endif()
if(ENABLE_RTOS_METRICS AND ENABLE_RTOS_CONTEXT_SWITCH_METRICS)
    list(APPEND PROJECT_DEFINES RTOS_CONTEXT_SWITCH_METRICS_ENABLE=1)
else()
    list(APPEND PROJECT_DEFINES RTOS_CONTEXT_SWITCH_METRICS_ENABLE=0)
endif()
if(ENABLE_RTOS_METRICS AND ENABLE_RTOS_METRICS_LOGGING AND
   ENABLE_RTOS_STACK_WATERMARK_METRICS)
    list(APPEND PROJECT_DEFINES RTOS_STACK_WATERMARK_METRICS_ENABLE=1)
else()
    list(APPEND PROJECT_DEFINES RTOS_STACK_WATERMARK_METRICS_ENABLE=0)
endif()
if(ENABLE_RTOS_STACK_OVERFLOW_DIAGNOSTICS)
    list(APPEND PROJECT_DEFINES RTOS_STACK_OVERFLOW_DIAGNOSTICS_ENABLE=1)
else()
    list(APPEND PROJECT_DEFINES RTOS_STACK_OVERFLOW_DIAGNOSTICS_ENABLE=0)
endif()
if(ENABLE_CONTROL_PIPELINE)
    list(APPEND PROJECT_DEFINES CONTROL_PIPELINE_ENABLE=1)
else()
    list(APPEND PROJECT_DEFINES CONTROL_PIPELINE_ENABLE=0)
endif()
if(ENABLE_DSHOT)
    list(APPEND PROJECT_DEFINES MOTOR_PROTOCOL_DSHOT=1)
else()
    list(APPEND PROJECT_DEFINES MOTOR_PROTOCOL_DSHOT=0)
endif()
if(ENABLE_MTF01P_DEBUG_PRINT)
    list(APPEND PROJECT_DEFINES MTF01P_DEBUG_PRINT_ENABLE=1)
else()
    list(APPEND PROJECT_DEFINES MTF01P_DEBUG_PRINT_ENABLE=0)
endif()

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
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc
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
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c
    FATFS/App/fatfs.c
    FATFS/Target/bsp_driver_sd.c
    FATFS/Target/fatfs_platform.c
    FATFS/Target/sd_diskio.c
    Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
    Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc.c
    Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_bot.c
    Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_data.c
    Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_scsi.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    Middlewares/Third_Party/FatFs/src/diskio.c
    Middlewares/Third_Party/FatFs/src/ff.c
    Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
    Middlewares/Third_Party/FatFs/src/option/ccsbcs.c
    Middlewares/Third_Party/FatFs/src/option/syscall.c
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    Middlewares/Third_Party/FreeRTOS/Source/list.c
    Middlewares/Third_Party/FreeRTOS/Source/queue.c
    Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    Middlewares/Third_Party/FreeRTOS/Source/timers.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    USB_DEVICE/App/usb_device.c
    USB_DEVICE/App/usbd_cdc_if.c
    USB_DEVICE/App/usbd_desc.c
    USB_DEVICE/App/usbd_storage_if.c
    USB_DEVICE/App/usb_mode.c
    USB_DEVICE/Target/usbd_conf.c
)

set(ASM_SOURCES
    Core/Startup/startup_stm32h743zitx.s
)

# Fault capture can run after a task-stack failure. Keep the recorder compact,
# integer-register-only, and independent of lazy FPU stacking.
set_source_files_properties(
    Core/Src/fault_diagnostics/fault_diagnostics.c
    PROPERTIES COMPILE_OPTIONS "-O2;-mgeneral-regs-only")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32H743ZITX_FLASH.ld)

add_executable(${PROJECT_NAME}.elf ${APP_SOURCES} ${LIB_SOURCES} ${ASM_SOURCES})

set(PARAMETER_GENERATION_STAMP
    "${CMAKE_BINARY_DIR}/parameter_generation.stamp")
add_custom_command(
    OUTPUT "${PARAMETER_GENERATION_STAMP}"
    BYPRODUCTS ${PARAMETER_GENERATED_OUTPUTS}
    COMMAND "${Python3_EXECUTABLE}" "${PARAMETER_GENERATOR}"
    COMMAND "${CMAKE_COMMAND}" -E touch "${PARAMETER_GENERATION_STAMP}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    DEPENDS
        "${PARAMETER_GENERATOR}"
        ${PARAMETER_MANIFESTS}
        "${CMAKE_SOURCE_DIR}/Core/Src/flight_controls/fcsModelAutocode/fcs_params.h"
    COMMENT "Generating and validating parameter catalogs"
    VERBATIM
)
add_custom_target(generate_parameters
    DEPENDS "${PARAMETER_GENERATION_STAMP}")
add_dependencies(${PROJECT_NAME}.elf generate_parameters)

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
    $<$<COMPILE_LANGUAGE:C>:-O2>
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
