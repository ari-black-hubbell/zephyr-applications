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

*Best practices for beginners*
- Start from one of the 600+ supported boards.
- Lean on vendor tooling. The command line is a great resource for more experienced developers, but vendor tooling (like JLink, Simplicity Commander, nRF Connect, and VS Code extensions) can be good for hitting the ground running.
- Follow training modules, or follow along with sample projects.
- Explore the tests and samples folders. There are samples for much of the functionality in Zephyr, and there are mandatory tests for each feature to get added to the main branch. If there are no samples for a certain feature, then you can check the tests folder for samples.
- Golioth -- provides free monthly live and asynchronous training, and hosts informational webcasts about Zephyr and how to use it. I attended one of these and found it very insightful.



## projects in this repo

*Building with west*
- For SiLabs efr32mg24: `west build -p always -b efr32_radio/efr32mg24b220f1536im48 samples/basic/blinky`
- For Nordic nrf52840dk: `west build -p always -b nrf52840dk/nrf52840 samples/basic/blinky`
- For Nordic nrf5340dk: `west build -p always -b nrf5340dk/nrf5340/cpuapp samples/basic/blinky`
- For STM stm32wb5mmdk: `west build -p always -b stm32wb5mm_dk samples/hello_world`

- *Note: nrf5340dk and stm32wb5mmdk are incompatible with the emulator in its current state (8/1/24), most likely due to simple hardware limitation errors (i.e., implemented functionality that does not check that the current board contains 2 or more LEDs, so the program does not successfully flash).*

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






