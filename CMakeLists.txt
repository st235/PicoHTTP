cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PICO_BOARD pico_w)

project(pico_http VERSION 0.1.0 DESCRIPTION "Lightweight http protocol implementation.")

include(FetchContent)

add_library(pico_http INTERFACE)

target_include_directories(pico_http INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_sources(pico_http INTERFACE
  src/http_server.cpp
  src/http_utils.h
  src/http_version.h
  src/http_coder.cpp
  src/log.h
  src/string_utils.h
  src/tcp_server.cpp
  src/tcp_connection.cpp
  src/http_response.cpp
)

# Fetch URI parser.
FetchContent_Declare(
  URIC
  GIT_REPOSITORY git@github.com:st235/URIC.git
  GIT_TAG "main"
  GIT_SHALLOW TRUE
  GIT_PROGRESS ON
)
FetchContent_MakeAvailable(URIC)

# Bring pico sdk dependencies.
target_link_libraries(pico_http INTERFACE
  uric
  pico_cyw43_arch_lwip_threadsafe_background
  pico_stdlib
)

# Test targets, disabled by default.
option(COMPILE_TESTS "Compiled with tests when turned on." OFF)
if (COMPILE_TESTS)
  message("Tests compilation enabled.")

  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )

  # For Windows: Prevent overriding the parent project's compiler/linker settings.
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  enable_testing()

  add_executable(pico_http_tests
    tests/url_tests.cpp
    tests/http_parser_tests.cpp
    tests/string_utils_tests.cpp
  )

  # GMock is required to use EXPECT_THAT.
  target_link_libraries(pico_http_tests PRIVATE GTest::gtest_main GTest::gmock_main)

  # Hacky way to add only classes
  # that do not depend on Pico SDK.
  target_sources(pico_http_tests PRIVATE
    include/url.h
    src/string_utils.h
  )

  include(GoogleTest)
  gtest_discover_tests(pico_http_tests)
endif()
