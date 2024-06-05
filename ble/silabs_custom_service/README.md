
# silabs_custom_service

## Description
Implementation of a custom BLE service with two custom characteristics, adapted from the Nordic custom service project within the same parent directory.

Development and testing performed on an EFR32xG24B.

## Development
The general structure for the development of this project was as follows:
1. Set project configuration.
2. Enable the Bluetooth host stack.
3. Set up connection callbacks.
4. Create the custom service.
5. Configure advertisement packets and scan response data.
6. Send the data via the established connection.
7. Test the BLE service.

## Structure
Configuration files and `bf.sh` (the bash script used for my local system to automatically build the project and flash it to my board) lie within the root directory. 

The source file is in `src/` and the files responsible custom service code are in `services/`.
