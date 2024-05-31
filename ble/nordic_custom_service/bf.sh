
#!/bin/zsh

## cd to the correct directory
cd ~/zephyrproject

## build/compile the script for the board in use
west build -p always -b nrf52840dk_nrf52840 ~/zephyr-applications/ble/nordic_custom_service

## flash the board
west flash --recover 