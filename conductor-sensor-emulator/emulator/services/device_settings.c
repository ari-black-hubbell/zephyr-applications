
/* device_settings.c 
 * A custom service consisting of the following write-only characteristics:
 *    LED, Fault Current, Low Battery Count, Alarm Timeout, Alarm Clear.  
 */


#include "services.h"

/* a bitmask to turn on specific lights. 
 * bits 7-2 reserved, bit 1 red LED (1 on / 0 off),
 * bit 0 green LED (1 on / 0 off).
 */
uint8_t led_value = 0x00;   


/* function called whenever RX characteristic is written to by a client. 
 * unpacks and outputs received data.
 */
static ssize_t on_receive(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                const void *buf, uint16_t len, uint16_t offset, uint8_t flags) {
                    // printk("led len: %d\n", sizeof(led_value));

                    printk("===============================\n");

                    /* print received data */
                    printk("data received: ");
                    const uint8_t * buffer = buf;
                    for(uint8_t i = 0; i < len; i++) { printk("%02X", buffer[i]);} 
                    printk("\n");

                    /* write data to target */
                    uint8_t *value = attr->user_data;
                    memcpy(value, buf, len);        /* TODO: unsafe, add buffer overflow prevention here */

                    /* print data written to the target */
                    printk("value: ");
                    for(uint8_t i = 0; i < len; i++) { printk("%02X", value[i]);}
                    printk("\n\n");

                    /* print bitmask 
                     * note: can add testing later using CLI interface using read/getter functions for 
                     * values otherwise unreadable.
                     */
                    /* operation output is 2 if true 0010 */
                    printk("red LED on: %d\n", (led_value & BIT(1)) == 2);  
                    /* operation output is 1 if true 0001 */
                    printk("green LED on: %d\n\n", (led_value & BIT(0)) == 1); 

                    printk("===============================\n");

	                return len;

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
BT_GATT_SERVICE_DEFINE(DS,                                         /* service name */
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_128(BT_UUID_DS)),      /* service UUID */ 

    /* attempt to add a descriptor / name for the custom service, unsuccessful. */
    // BT_GATT_DESCRIPTOR(     BT_UUID_DECLARE_128(BT_UUID_DS_DES),
    //                         BT_GATT_PERM_READ, read_ct, NULL, NULL),
    // BT_GATT_CUD(            "Device Settings", BT_GATT_PERM_READ),

    /* LED characteristic */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_LED),    /* characteristic UUID */  
                            BT_GATT_CHRC_WRITE |                    /* properties */
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP, 
                            BT_GATT_PERM_WRITE,                     /* permissions */
                            NULL, on_receive,                       /* callbacks (read, write) */
                            &led_value                              /* user data */
    ),
    /* LED descriptor */
    BT_GATT_CUD("LED", BT_GATT_PERM_READ),                      /* read only */

    /* Fault Current characteristic */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_FC),     
                            BT_GATT_CHRC_WRITE | 
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                            BT_GATT_PERM_WRITE,
                            NULL, on_receive,
                            NULL
    ),   
    /* Fault Current descriptor */
    BT_GATT_CUD("Fault Current", BT_GATT_PERM_READ),

    /* Low Battery Count characteristic */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_LBC),     
                            BT_GATT_CHRC_WRITE | 
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                            BT_GATT_PERM_WRITE,
                            NULL, on_receive,
                            NULL
    ),   
    /* Low Battery Count descriptor */
    BT_GATT_CUD("Low Battery Count", BT_GATT_PERM_READ),

    /* Alarm Timeout characteristic */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_ALT),     
                            BT_GATT_CHRC_WRITE | 
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                            BT_GATT_PERM_WRITE,
                            NULL, on_receive,
                            NULL
    ),   
    /* Alarm Timeout descriptor */
    BT_GATT_CUD("Alarm Timeout", BT_GATT_PERM_READ),

    /* Alarm Clear characteristic */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_ALC),     
                            BT_GATT_CHRC_WRITE | 
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                            BT_GATT_PERM_WRITE,
                            NULL, on_receive,
                            NULL
    ),   
    /* Alarm Clear descriptor */
    BT_GATT_CUD("Alarm Clear", BT_GATT_PERM_READ),

);



