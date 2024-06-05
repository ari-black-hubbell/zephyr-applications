
#!/bin/zsh

## cd to the correct directory
cd ~/zephyrproject

## build/compile the script for the board in use
west build -p always -b efr32xg24_dk2601b ~/zephyr-applications/ble/silabs_custom_service

## flash the board
west flash 