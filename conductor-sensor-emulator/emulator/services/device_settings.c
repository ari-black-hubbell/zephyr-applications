
/* device_settings.c 
 * A custom service consisting of the following write-only characteristics:
 *    LED, Fault Current, Low Battery Count, Alarm Timeout, Alarm Clear.  
 */

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

#include "services.h"


/* initialize data structures */
uint8_t data[MAX_TRANSMIT_SIZE];        /* transmission buffer to hold data. */
static uint8_t ct[16];




/* Service initialization function to execute after connection is established.
 * 
 * Returns: 0 on success, != 0 otherwise.
 */
int init_service(void) {

    /* instantiate local variables */
    int err = 0;    /* to hold return codes. */

    /* initialize transmission buffer to 0 */
    memset(&data, 0, MAX_TRANSMIT_SIZE);

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
                    return len;
                }

static ssize_t read_ct(struct bt_conn *conn, const struct bt_gatt_attr *attr,
		       void *buf, uint16_t len, uint16_t offset)
{
	const char *value = "Device Settings";

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 sizeof(ct));
}
                

/* function called whenever the client changes the CCCD register. */
void on_cccd_changed(const struct bt_gatt_attr *attr, uint16_t value) {
    ARG_UNUSED(attr);
    switch(value) {
        // if notify
        case BT_GATT_CCC_NOTIFY:
            // start sending stuff
            printk("Enabling notifications\n");
            break;
        // if indicate
        case BT_GATT_CCC_INDICATE:
            // start sending stuff via indications
            printk("Enabling indications\n");
            break;
        // if 0 / exit
        case 0:
            // stop sending stuff
            printk("Disabling notifications and indications\n");
            break;
        // otherwise
        default:
            printk("Error, CCCD has been set to an invalid value");
    }
}

/* A macro to statically register the service in the host stack. 
 */
BT_GATT_SERVICE_DEFINE(DS,                                      /* the name of the service. */
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_128(BT_UUID_DS)),                        /* the primary service UUID. */ 
    // BT_GATT_SECONDARY_SERVICE
    // BT_GATT_SERVICE

    BT_GATT_DESCRIPTOR(     BT_UUID_DECLARE_128(BT_UUID_DS_DES),            // not showing up :(
                            BT_GATT_PERM_READ, read_ct, NULL, NULL),
    BT_GATT_CUD(            "Device Settings", BIT(0)), /* read only */

    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_LED),       // characteritic UUID (transmitting)
                            BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                            BT_GATT_PERM_WRITE, NULL, on_receive, NULL),              // callbacks (read, write, user data)),
    BT_GATT_CUD(            "LED", BT_GATT_PERM_READ), /* read only */

    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_FC),       // characteritic UUID (transmitting)
                            BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                            BT_GATT_PERM_WRITE, NULL, on_receive, NULL),              // callbacks (read, write, user data)),
    BT_GATT_CUD(            "Fault Current", BT_GATT_PERM_READ), /* read only */

    // BT_GATT_CCC(on_cccd_changed,                        // client characteristic configuration (when config changed)
    //         BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),    // hold information about read/write permissions

);




// /* function for sending notifications to a client with the provided data, given that the 
//  * Client Characteristic Control Descriptor has been set to notify (0x1).
//  * calls on_sent() callback if successful.
// */
// void my_service_send(struct bt_conn *conn, const uint8_t *data, uint16_t len) {
//     /* attribute table: 0 = primary service, 1 = RX properties, 2 = RX descriptor, 
//      * 3 = TX properties, 4 = TX descriptor, 5 = CCC. 
//      */
//     // define attribute structure
//     const struct bt_gatt_attr *attr = &DS.attrs[3];
//     // define params structure
//     struct bt_gatt_notify_params params = {
//         .uuid   = BT_UUID_MY_SERVICE_TX,
//         .attr   = attr,
//         .data   = data,
//         .len    = len,
//         .func   = on_sent
//     };

//     // check if notifications are enabled
//     // if (bt_gatt_is_subscribed(conn, attr, BT_GATT_CCC_NOTIFY)) {     // always evaluates to FALSE, either (my) dev bug or Zephyr bug

//         // send the notification
//         if (bt_gatt_notify_cb(conn, &params)) { printk("Error, unable to send notification\n"); }
        
//     // }
//     // else display a warning
//     // else { printk("Warning, notification not enabled on the selected attribute\n"); }
    
// }


