cmake_minimum_required(VERSION 3.21)
project(cclaw_msvc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

find_package(CURL REQUIRED)

add_executable(cclaw
  src/main.c
  src/cli.c
  src/config.c
  src/cron.c
  src/daemon.c
  src/fs.c
  src/gateway.c
  src/memory.c
  src/organize.c
  src/provider.c
  src/platform_win.c
)

target_include_directories(cclaw PRIVATE include)
target_link_libraries(cclaw PRIVATE CURL::libcurl Ws2_32)
