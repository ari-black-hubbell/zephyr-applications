# conductor-sensor-emulator

An embedded software project aiming to successfully emulate signals from Conductor Sensors such that data from the emulator is indistinguishable from true sources, with minimal to no required adjustments to enable its use across boards from different manufacturers (e.g., the three boards/DKs used in the BLE custom service projects in this repository, from Nordic, SiLabs, and STMicroelectronics).

---

### Some requirements for this project include:

#### Must utilize the appropriate wireless security protocols.
- Via AES-CCM cryptography for encryption (optional) and the authentication scheme described in the BLE Interface specification document (7.1, required).

#### Must implement the GATT (General ATTribute profile) database.
- By defining a database that contains data relevant to all characteristics associated with the GATT server, which may include the following attributes:
  - The characteristic value.
  - The characteristic description (a descriptor storing the properties, location, and type of the characteristic value).
  - The Client Characteristic Configuration (a configuration that allows the GATT server to configure the characteristic to be notified or indicated).
 - Note that the following properties are asociated with each attribute:
  - A handle (index).
  - Its type (indicates what the attribute data represents, a UUID).
  - Permissions (enforces if/how a GATT client device can access the attribute's value).
- All required characteriistics are in section 6.1.

#### Must support advertising according to the specifications in the document.
- Section 4.
  
