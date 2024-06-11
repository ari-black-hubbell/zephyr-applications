
# silabs_custom_service

## Description
Implementation of a custom BLE service with two custom characteristics, adapted from the Nordic custom service project within the same parent directory. Note that the modifications to port the code from the Nordic board to the Silabs board were very minimal (including only an optional name change and updated bash scripts), indicating that little to no work must be done to adapt code for a custom BLE service to fit a Zephyr-supported board developed by Silicon Labs, despite its general lack of compatability with the RTOS overall.

Development and testing performed on an EFR32xG24B + 20 dBm Pro Kit, xG24-PK6010A. The mainboard was Silabs' BRD4002A, and the radio board was their BRD4187C.

## Development
The general structure for the development of this project was as follows:
1. Determine whether it is possible to build and flash software using Zephyr on the obtained Silicon Labs board.
2. Make minor adjustments to the code from the Nordic project to better fit the Silabs board.
3. Test the custom service.

## Structure
Configuration files and `b.sh` (the bash script used for my local system to automatically build the project) lie within the root directory. 

The source file is in `src/` and the files responsible custom service code are in `services/`.
