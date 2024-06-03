
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

/* function called whenever RX characteristic is written to by a client. 
 * unpacks and outputs received data.
*/
static ssize_t on_receive(struct bt_conn *conn,
                const struct bt_gatt_attr *attr,
                const void *buf,
                uint16_t len,
                uint16_t offset,
                uint8_t flags) {
                    const uint8_t * buffer = buf;
                    printk("Received data, handle %d, conn %p, data 0x", attr->handle, conn);
                    for(uint8_t i = 0; i < len; i++) { printk("%02X", buffer[i]);}              // iterate through buffer, print all data
                    printk("\n");
                    retrn len;
                }

/* function called whenever a notification is sent by the TX characteristic. 
 * outputs the address of the recipient.
*/
static void on_sent(struct *conn, void *user_data) {
    ARG_UNUSED(user_data);
    const bt_addr_le_t * addr = bt_conn_get_dst(conn);                                  // get address/destination of the connection endpoint
    printk("Data sent to Address 0x %02X %02X %02X %02X %02X %02X \n", addr->a.val[0],  // display the address
                                                                        addr->a.val[1],
                                                                        addr->a.val[2],
                                                                        addr->a.val[3],
                                                                        addr->a.val[4],
                                                                        addr->a.val[5]);
}

/* function called whenever the client changes the CCCD register. */
void on_cccd_changed(const struct bt_gatt_attr *attr, uint16_t value) {
    ARG_UNUSED(attr);
    switch(value) {
        // if notify
        case BT_GATT_CCC_NOTIFY:
            // start sending stuff
            break;
        // if indicate
        case BT_GATT_CCC_INDICATE:
            // start sending stuff via indications
            break;
        // if 0 / exit
        case 0;
            // stop sending stuff
        // otherwise
        default:
            printk("Error, CCCD has been set to an invalid value");
    }
}

/* function for sending notifications to a client with the provided data, given that the 
 * Client Characteristic Control Descriptor has been set to notify (0x1).
 * calls on_sent() callback if successful.
*/
void my_service_send(struct bt_conn *conn, const uint8_t *data, uint16_t len) {
    /* attribute table: 0 = service, 1 = primary service, 2 = RX, 3 = TX, 4 = CCC. */
    // define attribute structure
    const struct bt_gatt_attr *attr = &my_service.attrs[3];
    // define params structure
    struct bt_gatt_notify_params params = {
        .uuid   = BT_UUID_MY_SERVICE_TX,
        .attr   = attr,
        .data   = data,
        .len    = len,
        .func   = on_sent
    };
    // check if notifications are enabled
    if (bt_gatt_is_subscribed(conn, attr, BT_GATT_CCC_NOTIFY)) {
        // send the notification
        if (bt_gatt_notify_cb(conn, &params)) { printk("Error, unable to send notification\n"); }
    }
    // else display a warning
    else { printk("Warning, notification not enabled on the selected attribute\n"); }
}