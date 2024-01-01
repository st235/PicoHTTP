cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)

# Initialise PicoSDK as submodule.
include(pico-sdk/pico_sdk_init.cmake)

project(pico_http VERSION 0.0.1 DESCRIPTION "Lightweight http protocol implementation.")

pico_sdk_init()

add_library(pico_http
  src/http_server.cpp
  src/http_utils.h
  src/http_version.h
  src/http1_1_parser.h
  src/http1_1_parser.cpp
  src/log.h
  src/string_utils.h
  src/tcp_server.h
  src/tcp_server.cpp
  src/tcp_connection.h
  src/tcp_connection.cpp
)

target_include_directories(pico_http PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(pico_http PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

target_link_libraries(pico_http
  pico_cyw43_arch_lwip_threadsafe_background
  pico_stdlib
)
