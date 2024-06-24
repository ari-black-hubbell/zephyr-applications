cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(conductor-sensor-emulator)

target_sources(app PRIVATE
    src/main.c
    services/services.c
    services/device_settings.c
    services/sensor_info.c
    services/wireless_security.c
)

zephyr_library_include_directories(.)