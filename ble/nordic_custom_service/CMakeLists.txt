cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(nordic_custom_service)

target_sources(app PRIVATE src/main.c services/my_service.c)

zephyr_library_include_directories(.)