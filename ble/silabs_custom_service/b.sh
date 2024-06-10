
#!/bin/zsh

## cd to the correct directory
cd ~/zephyrproject

## build/compile the script for the board in use
west build -p always -b efr32_radio/efr32mg24b220f1536im48 ~/zephyr-applications/ble/silabs_custom_service

## note: need to flash the board using Simplicity Studio's Commander
    ## set debug mode to MCU
    ## erase the chip
    ## uncheck "reset MCU after flashing"
    ## flash the .elf file in zephyr/build/zephyr/
    ## view serial output with: picocom -b 115200 /dev/tty.usbmodem0004403016981
