# conductor-sensor-emulator

An embedded software project aiming to successfully emulate signals from Conductor Sensors such that data from the emulator is indistinguishable from true sources, with minimal to no required adjustments to enable its use across boards from different manufacturers.

More importantly, however, the project is meant to serve as a proof of concept to showcase Zephyr's feasibility as an alternative to traditional ecosystems for embedded systems development at Hubbell. 

---

### Some features of the project in its current state include:

#### BLE advertising support according to the appropriate BLE Interface specification document (4).
- Advertises relevant device information to establish a connection with the BLE-connected device.

#### Basic framework implementations of each custom service and characteristic.
- Can send and receive data to store or read values for characteristics belonging to the Device Settings, Sensor Information, and Wireless Security custom services' characteristics.
- LEDs can be toggled via sending bitmask values to Device Service LED characteristic, for supported boards. Note that this feature's implementation may result in a lack of support for other boards due to hardware limitations (i.e., having less than two LEDs to toggle on and off).

#### Demonstrated hash calculation using Zephyr's cryptography library.
- Can perform a SHA-256 calculation and reliably obtain the expected hash, which suggests that the adequate authentication scheme and wireless security protocols expected for the conductor sensor (and other products) can be implemented successfully.
- (Minor) further work on this project would allow the emulator to perform the hash using a value read from the Initialization Vector characteristic, and further development could implement a proper HMAC authentication scheme.
- Note that encryption may also be implemented rather easily, as shown in `temp/`.
