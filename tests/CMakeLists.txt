cmake_minimum_required(VERSION 3.22)

project(customAutopilotHostTests LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(gps_itow_deduplicator_test
    sensors/ubloxm9n_rb/gps_itow_deduplicator_test.cc)
target_include_directories(gps_itow_deduplicator_test PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/../Core/Src/sensors/ubloxm9n_rb")
target_compile_options(gps_itow_deduplicator_test PRIVATE
    -Wall
    -Wextra
    -Werror)

add_test(NAME gps_itow_deduplicator COMMAND gps_itow_deduplicator_test)
