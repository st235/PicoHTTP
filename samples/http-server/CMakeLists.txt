cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PICO_BOARD pico_w)

# Initialise PicoSDK as submodule.
include(${CMAKE_CURRENT_BINARY_DIR}/../../pico-sdk/pico_sdk_init.cmake)
pico_sdk_init()

add_subdirectory(../.. ${CMAKE_CURRENT_BINARY_DIR}/pico-http)

project(http_server)

if((NOT WIFI_SSID) OR (NOT WIFI_PASSWORD))
    message(FATAL_ERROR "WIFI_SSID or WIFI_PASSWORD are not set")
endif((NOT WIFI_SSID) OR (NOT WIFI_PASSWORD))

add_executable(http_server
    http_server.cpp
)

target_link_libraries(http_server
    pico_stdlib
    pico_cyw43_arch_none
    pico_http
)

target_compile_definitions(http_server PRIVATE
    WIFI_SSID=\"${WIFI_SSID}\"
    WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
)

pico_enable_stdio_usb(http_server 1)
pico_enable_stdio_uart(http_server 0)

pico_add_extra_outputs(http_server)
