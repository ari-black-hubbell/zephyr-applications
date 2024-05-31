
/* my_service.c */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

#include <soc.h>
#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "../services/my_service.h"


/* macros */
#define MAX_TRANSMIT_SIZE 240                                                   // the length of transmission buffers.
#define BT_UUID_MY_SERVICE      BT_UUID_DECLARE_128(MY_SERVICE_UUID)            // the service custom UUID.
#define BT_UUID_MY_SERVICE_TX   BT_UUID_DECLARE_128(TX_CHARACTERISTIC_UUID)     // the transmission characteristic custom UUID.
#define BT_UUID_MY_SERVICE_RX   BT_UUID_DECLARE_128(RX_CHARACTERISTIC_UUID)     // the receiving characteristic custom UUID.

/* helper macro to statically register the service in the host stack. 
 * LED button service declaration and registration.
*/
BT_GATT_SERVICE_DEFINE(my_service,                                          // name
BT_GATT_PRIMARY_SERVICE(BT_UUID_MY_SERVICE),                                // primary service UUID (service ID)
BT_GATT_CHARACTERISTIC(BT_UUID_MY_SERVICE_RX,                               // characteristic UUID (receiving)   
                    BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,   // properties (value is writable with/without response) 
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,                 // permissions (value can be read / written to)
                    NULL, on_receive, NULL),                                // callbacks (read, write, user data)
BT_GATT_CHARACTERISTIC(BT_UUID_MY_SERVICE_TX,       // characteritic UUID (transmitting)
                    BT_GATT_CHRC_NOTIFY,            // properties (permits notifications on value change without acknowledgement)
                    BT_GATT_PERM_READ,              // permissions (value can be read)
                    NULL, NULL, NULL),              // callbacks (read, write, user data)
BT_GATT_CCC(lbslc_ccc_cfg_changed,                  // client characteristic configuration (when config changed)
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),    // hold information about read/write permissions
);

/* transmission buffer definitions */
uint8_t data_tx[MAX_TRANSMIT_SIZE];     // trasmission buffer (for transmitting).
uint8_t data_rx[MAX_TRANSMIT_SIZE];     // trasmission buffer (for receiving).

/* initialization function to call from main. */
int my_service_init(void) {
    // initialize a value to return
    int err = 0;
    // initialize transmission buffers to 0
    memset(&data_rx, 0, MAX_TRANSMIT_SIZE);
    memset(&data_tx, 0, MAX_TRANSMIT_SIZE);
    // return output value
    return err;


}