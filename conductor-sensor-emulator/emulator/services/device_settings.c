
/* device_settings.c 
 * A custom service consisting of the following write-only characteristics:
 *    LED, Fault Current, Low Battery Count, Alarm Timeout, Alarm Clear.  
 */


#include "services.h"


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
                            NULL                                    /* user data */
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



