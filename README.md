# zephyr-applications

A collection of simple projects and sample applications developed for Zepher OS.


-------

# resources

## zephyr

[Zephyr Project documentation](https://docs.zephyrproject.org/latest/index.html)
- Contains lots of introductory (and more advanced) information about Zephyr and its API, as well as a comprehensive Getting Started guide.

[Zephyr GitHub page](https://github.com/zephyrproject-rtos/zephyr)
- Contains all of Zephyr project’s source code.

[Sample project for implementing Zephyr BLE](https://devzone.nordicsemi.com/guides/nrf-connect-sdk-guides/b/getting-started/posts/ncs-ble-tutorial-part-1-custom-service-in-peripheral-role)
- The reference I used to help develop my BLE application.
- Incredibly helpful, though maybe not entirely error-free or up to date.

[Zephyr porting guide](https://docs.zephyrproject.org/latest/hardware/porting/board_porting.html)
- Although I didn’t really get to use it, it would be cool to return to this page in the future to add Zephyr support to more boards.


## projects in this repo

*Building with west*
- For SiLabs efr32mg24: `west build -p always -b efr32_radio/efr32mg24b220f1536im48 samples/basic/blinky`
- For Nordic nrf52840dk: `west build -p always -b nrf52840dk/nrf52840 samples/basic/blinky`
- For STM stm32wb5mmdk: `west build -p always -b stm32wb5mm_dk samples/hello_world`

*Flashing*
- For SiLabs:
  1. Open Simplicity Commander, select the right kit, and connect.
  2. Go to the *Flash* tab, change the binary file path to Zephyr's build path (.elf file) and flash. This may look like `/Users/<user>/zephyrproject/build/zephyr/zephyr.elf`
 
- For other boards:
  `west flash`


## relevant fundamentals

[BLE fundamentals course](https://academy.nordicsemi.com/courses/bluetooth-low-energy-fundamentals/)
- Very informative with clear and concise explanations.
- (Also free!)


