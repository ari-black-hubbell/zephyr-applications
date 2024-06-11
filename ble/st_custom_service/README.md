
# st_custom_service

## Description
Implementation of a custom BLE service with two custom characteristics, adapted from the Nordic custom service project within the same parent directory. Note that the modifications to port the code from the Nordic board to the ST board were very minimal (including only an optional name change and updated bash scripts), indicating that little to no work must be done to adapt code for a custom BLE service to fit a Zephyr-supported board developed by ST, despite its general lack of compatability with the RTOS overall.

Development and testing performed on an STM32WB5MM-DK using the LightBlue mobile application to connect a client device to the peripheral.

## Development
The general structure for the development of this project was as follows:
1. Make minor adjustments to the code from the Nordic project to better fit the ST board.
2. Test the custom service.


## Structure
Configuration files and `bf.sh` (the bash script used for my local system to automatically build the project and flash it to my board) lie within the root directory. 

The source file is in `src/` and the files responsible custom service code are in `services/`.
