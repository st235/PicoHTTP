cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)

# Initialise PicoSDK as submodule.
include(${CMAKE_CURRENT_BINARY_DIR}/../../pico-sdk/pico_sdk_init.cmake)

add_subdirectory(.. ${CMAKE_CURRENT_BINARY_DIR}/pico-http)

project(example)

pico_sdk_init()

if((NOT WIFI_SSID) OR (NOT WIFI_PASSWORD))
    message(FATAL_ERROR "WIFI_SSID or WIFI_PASSWORD are not set")
endif(NOT WIFI_SSID OR NOT WIFI_PASSWORD)

add_executable(example
    picow_http_example.cpp
)

target_link_libraries(example
    pico_stdlib
    pico_cyw43_arch_none
    pico_http
)

target_compile_definitions(example PRIVATE
    WIFI_SSID=\"${WIFI_SSID}\"
    WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
)

pico_enable_stdio_usb(example 1)
pico_enable_stdio_uart(example 0)

pico_add_extra_outputs(example)
