cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PICO_BOARD pico_w)

project(pico_http VERSION 0.1.0 DESCRIPTION "Lightweight http protocol implementation.")

add_library(pico_http INTERFACE)

target_sources(pico_http INTERFACE
  src/http_server.cpp
  src/http_utils.h
  src/http_version.h
  src/http1_1_parser.cpp
  src/log.h
  src/string_utils.h
  src/tcp_server.cpp
  src/tcp_connection.cpp
)
target_include_directories(pico_http INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Bring pico sdk dependencies.
target_link_libraries(pico_http INTERFACE
  pico_cyw43_arch_lwip_threadsafe_background
  pico_stdlib
)
