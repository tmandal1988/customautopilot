set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(
    _STM32_EXPECTED_GCC_BANNER
    "arm-none-eabi-gcc (GNU Tools for STM32 13.3.rel1.20240926-1715) 13.3.1 20240614"
)
set(
    STM32_TOOLCHAIN_BIN_DIR
    ""
    CACHE PATH
    "Directory containing GNU Tools for STM32 13.3 executables"
)
list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES STM32_TOOLCHAIN_BIN_DIR)

set(_STM32_TOOLCHAIN_SOURCE "")
if(STM32_TOOLCHAIN_BIN_DIR)
    set(_STM32_TOOLCHAIN_CANDIDATE "${STM32_TOOLCHAIN_BIN_DIR}")
    set(_STM32_TOOLCHAIN_SOURCE "CMake cache")
elseif(DEFINED ENV{STM32_TOOLCHAIN_BIN_DIR}
       AND NOT "$ENV{STM32_TOOLCHAIN_BIN_DIR}" STREQUAL "")
    file(
        TO_CMAKE_PATH
        "$ENV{STM32_TOOLCHAIN_BIN_DIR}"
        _STM32_TOOLCHAIN_CANDIDATE
    )
    set(_STM32_TOOLCHAIN_SOURCE "STM32_TOOLCHAIN_BIN_DIR environment variable")
else()
    set(_STM32_COMMON_TOOLCHAIN_DIRS
        "$ENV{HOME}/st/stm32cubeclt_1.18.0/GNU-tools-for-STM32/bin"
        "/opt/st/stm32cubeclt_1.18.0/GNU-tools-for-STM32/bin"
    )
    foreach(_STM32_COMMON_DIR IN LISTS _STM32_COMMON_TOOLCHAIN_DIRS)
        if(EXISTS "${_STM32_COMMON_DIR}/arm-none-eabi-gcc")
            set(_STM32_TOOLCHAIN_CANDIDATE "${_STM32_COMMON_DIR}")
            set(_STM32_TOOLCHAIN_SOURCE "standard STM32CubeCLT location")
            break()
        endif()
    endforeach()

    if(NOT _STM32_TOOLCHAIN_CANDIDATE)
        find_program(
            _STM32_GCC_FROM_PATH
            NAMES arm-none-eabi-gcc
            NO_CACHE
        )
        if(_STM32_GCC_FROM_PATH)
            get_filename_component(
                _STM32_TOOLCHAIN_CANDIDATE
                "${_STM32_GCC_FROM_PATH}"
                DIRECTORY
            )
            set(_STM32_TOOLCHAIN_SOURCE "PATH")
        endif()
    endif()
endif()

if(NOT _STM32_TOOLCHAIN_CANDIDATE)
    message(FATAL_ERROR
        "GNU Tools for STM32 13.3 was not found.\n"
        "Install STM32CubeCLT 1.18.0, add its GNU-tools-for-STM32/bin "
        "directory to PATH, or configure with:\n"
        "  -DSTM32_TOOLCHAIN_BIN_DIR=/path/to/GNU-tools-for-STM32/bin"
    )
endif()

set(_STM32_GCC     "${_STM32_TOOLCHAIN_CANDIDATE}/arm-none-eabi-gcc")
set(_STM32_GXX     "${_STM32_TOOLCHAIN_CANDIDATE}/arm-none-eabi-g++")
set(_STM32_OBJCOPY "${_STM32_TOOLCHAIN_CANDIDATE}/arm-none-eabi-objcopy")
set(_STM32_SIZE    "${_STM32_TOOLCHAIN_CANDIDATE}/arm-none-eabi-size")

foreach(_STM32_TOOL IN ITEMS
    "${_STM32_GCC}"
    "${_STM32_GXX}"
    "${_STM32_OBJCOPY}"
    "${_STM32_SIZE}"
)
    if(NOT EXISTS "${_STM32_TOOL}")
        message(FATAL_ERROR "Required STM32 toolchain executable not found: ${_STM32_TOOL}")
    endif()
endforeach()

execute_process(
    COMMAND "${_STM32_GCC}" --version
    RESULT_VARIABLE _STM32_GCC_RESULT
    OUTPUT_VARIABLE _STM32_GCC_VERSION_OUTPUT
    ERROR_VARIABLE _STM32_GCC_VERSION_ERROR
)
if(NOT _STM32_GCC_RESULT EQUAL 0)
    message(FATAL_ERROR "Unable to run ${_STM32_GCC}: ${_STM32_GCC_VERSION_ERROR}")
endif()

string(REGEX MATCH "^[^\r\n]*" _STM32_GCC_BANNER "${_STM32_GCC_VERSION_OUTPUT}")
if(NOT "${_STM32_GCC_BANNER}" STREQUAL "${_STM32_EXPECTED_GCC_BANNER}")
    message(FATAL_ERROR
        "Incorrect STM32 GNU toolchain.\n"
        "Expected: ${_STM32_EXPECTED_GCC_BANNER}\n"
        "Found:    ${_STM32_GCC_BANNER}\n"
        "Selected: ${_STM32_TOOLCHAIN_CANDIDATE}\n"
        "Set STM32_TOOLCHAIN_BIN_DIR to the exact 13.3.rel1 bin directory."
    )
endif()

set(
    STM32_TOOLCHAIN_BIN_DIR
    "${_STM32_TOOLCHAIN_CANDIDATE}"
    CACHE PATH
    "Directory containing GNU Tools for STM32 13.3 executables"
    FORCE
)
message(STATUS
    "STM32 toolchain (${_STM32_TOOLCHAIN_SOURCE}): ${_STM32_GCC_BANNER}"
)

set(CMAKE_C_COMPILER   "${_STM32_GCC}")
set(CMAKE_CXX_COMPILER "${_STM32_GXX}")
set(CMAKE_ASM_COMPILER "${_STM32_GCC}")
set(CMAKE_OBJCOPY      "${_STM32_OBJCOPY}" CACHE FILEPATH "STM32 objcopy executable" FORCE)
set(CMAKE_SIZE         "${_STM32_SIZE}"    CACHE FILEPATH "STM32 size executable" FORCE)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
