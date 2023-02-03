cmake_minimum_required(VERSION 3.13)

include(../pico-sdk/external/pico_sdk_import.cmake)
include(../pico-sdk/tools/CMakeLists.txt)

project(GC9A01_test C CXX)
set(CMAKE_C_STANDARD 11)
#set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(GC9A01_test  
    gc9a01_test.c
    GC9A01/gc9a01.c
)

target_link_libraries(GC9A01_test
    pico_stdlib
    hardware_spi
    hardware_adc
)

pico_enable_stdio_usb(GC9A01_test 0)
pico_enable_stdio_uart(GC9A01_test 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(GC9A01_test)

#RM
