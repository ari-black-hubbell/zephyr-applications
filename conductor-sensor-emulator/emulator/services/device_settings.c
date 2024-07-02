
/* device_settings.c 
 * A custom service consisting of the following write-only characteristics:
 *    LED, Fault Current, Low Battery Count, Alarm Timeout, Alarm Clear.  
 */


#include "services.h"

/* (LED) a bitmask to turn on specific lights. 
 * bits 7-2: Reserved
 * bit 1: 1 on / 0 off (red LED) 
 * bit 0: 1 on / 0 off (green LED) 
 */
uint8_t led_value = 0x00;   

/* (Fault Current) specifies the trigger level for the magnetometer in micro tesla.
 */
uint16_t fc_value = 0x0000;
// unsigned char fc_value = 0x0000;

/* (Low Battery Count) specifies the coulomb count at which to signal a low battery.
 */
uint16_t lbc_value = 0x0000;

/* (Alarm Timeout) specifies the number of hours before a transient alarm clears.
 */
uint8_t alt_value = 0x00;

/* (Alarm Clear) a bitmask of alarms to clear.
 * bit 7: Free-Fall Detected
 * bit 6: Fault Current Detected
 * bit 5: Galloping Line Detected -- reserved future use
 * bit 4: Low Battery
 * bit 3: Sensor Over Temp Detected
 * bit 2: Conductor Sag Detected
 * bit 1: Reserved
 * bit 0: Reserved
 */
uint8_t alc_value = 0x00;

/* REMOVE LATER, TEMP FOR TESTING*/
static ssize_t on_read(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset) {
    /* TODO: read value(s) from database */
	// uint8_t health_data = 0;    /* temporary value */
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &fc_value,
				 sizeof(fc_value));
}


/* function called whenever characteristic is written to by a client. 
 * unpacks and outputs received data.
 */
static ssize_t on_receive(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                const void *buf, uint16_t len, uint16_t offset, uint8_t flags) {

                    printk("===============================\n");

                    /* get handle */
                    uint16_t handle = bt_gatt_attr_get_handle(bt_gatt_find_by_uuid(NULL, 0, attr->uuid));
                    printk("handle: %d\n", handle);

                    /* write data to target (longer inputs) */
                    if (handle == 6 || handle == 9) {

                        /* do not accept inputs too large (4 bytes / 2 words) */
                        if (len > 2) { 
                            printk("(ERR) input value too large,\n maximum length is 2 words\n");
                            printk("===============================\n");
                            return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN); 
                        }

                        /* update the value */
                        unsigned char *value = attr->user_data;         /* previously type uint16_t*/
                        /* should we clear the current value (init to 0s) then write? */
                        // value = 0x0000; 
                        memcpy(value, buf, len);                        /* is this safe? */

                        /* output to console */
                        switch (handle) {
                            case 6: 
                                printk("fault current value successfully updated.\n"); 
                                break; 
                            case 9:
                                printk("low battery count value successfully updated.\n");
                                break;
                        }
                        // for(int i = 0; i < len; i++) {printk("%X", value[i]);}
                        
                    }

                    /* write data to target (shorter inputs) */
                    else {

                        /* do not accept inputs too large (2 bytes / 1 word) */
                        if (len > 1) { 
                            printk("(ERR) input value too large,\n maximum length is 1 word\n");
                            printk("===============================\n");
                            return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN); 
                        }
                        printk("len: %d\n", len);

                        /* update the value */
                        uint8_t *value = attr->user_data;
                        // value = 0x00;
                        memcpy(value, buf, len);                        /* is this safe? */

                        /* output to console */
                        switch (handle) {
                            case 3:
                                printk("value (led): ");
                                break;
                            case 12:
                                printk("value (alt): ");
                                break;
                            case 15:
                                printk("value (alc): ");
                                break;
                        }
                        for(uint8_t i = 0; i < len; i++) {printk("%X", value[i]);}
                    }

                    printk("\n\n");

                    /* print current configuration 
                     * note: can add testing later using CLI interface using read/getter functions for 
                     * values otherwise unreadable.
                     */

                    /* LED bitmask */
                    printk("red LED on: %d\n", (led_value & BIT(1)) == 2);      /* & outputs 2 if true 0010 */
                    printk("green LED on: %d\n", (led_value & BIT(0)) == 1);    /* & outputs 1 if true 0001 */

                    /* Fault Current value */
                    printk("fault current value: ");
                    char fc_arr[sizeof(fc_value)];
                    bt_gatt_attr_read(conn, attr, fc_arr, len, offset, &fc_value, sizeof(fc_value));
                    for(uint8_t i = 0; i < len; i++) {printk("%X", fc_arr[i]);}
                    printk(" mT\n");

                    /* Low Battery Count value */
                    printk("low battery value: :");
                    char lbc_arr[sizeof(lbc_value)];
                    bt_gatt_attr_read(conn, attr, lbc_arr, len, offset, &lbc_value, sizeof(lbc_value));
                    for(uint8_t i = 0; i < len; i++) {printk("%X", lbc_arr[i]);}
                    printk(" C\n");     /* what should the correct units be here? (coulomb counter counts) */


                    // printk("%d", fc_value);         // not printing correct value (?) try using actual read fn
                    // for(int i = 0; i < len; i++) {printk("%X", fc_value[i]);}
                    // printk(" mT\n");

                    /* print FC value */
                    // printk("fault current value: ");
                    // uint16_t value_16 = fc_value;
                    // for(uint8_t i = 0; i < len; i++) {printk("%02X", value_16[i]);}
                    // printk("\n");


                    printk("\n");

                    // char val[sizeof(fc_value)];
                    // bt_gatt_attr_read(conn, attr, val, len, offset, &fc_value, sizeof(fc_value));
                    // for(uint8_t i = 0; i < len; i++) {printk("%X", val[i]);}
                    // printk("\n");


                    printk("===============================\n");
                    // return bt_gatt_attr_read(conn, attr, buf, len, offset, &fc_value, sizeof(fc_value));

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

    /* LED characteristic, handle 3 */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_LED),    /* characteristic UUID */  
                            BT_GATT_CHRC_WRITE |                    /* properties */
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP, 
                            BT_GATT_PERM_WRITE,                     /* permissions */
                            NULL, on_receive,                       /* callbacks (read, write) */
                            &led_value                              /* user data */
    ),
    /* LED descriptor */
    BT_GATT_CUD("LED", BT_GATT_PERM_READ),                      /* read only */

    /* Fault Current characteristic, handle 6 */
    // BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_FC),     
    //                         BT_GATT_CHRC_WRITE | 
    //                         BT_GATT_CHRC_WRITE_WITHOUT_RESP,
    //                         BT_GATT_PERM_WRITE,
    //                         NULL, on_receive,
    //                         &fc_value
    // ),   

    /* DELETE LATER, TEMP FOR TESTING*/
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_FC),     
                            BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ |
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                            BT_GATT_PERM_WRITE | BT_GATT_PERM_READ,
                            on_read, on_receive,
                            &fc_value
    ), 
    /* Fault Current descriptor */
    BT_GATT_CUD("Fault Current", BT_GATT_PERM_READ),

    /* Low Battery Count characteristic, handle 9 */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_LBC),     
                            BT_GATT_CHRC_WRITE | 
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                            BT_GATT_PERM_WRITE,
                            NULL, on_receive,
                            &lbc_value
    ),   
    /* Low Battery Count descriptor */
    BT_GATT_CUD("Low Battery Count", BT_GATT_PERM_READ),

    /* Alarm Timeout characteristic, handle 12 */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_ALT),     
                            BT_GATT_CHRC_WRITE | 
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                            BT_GATT_PERM_WRITE,
                            NULL, on_receive,
                            &alt_value
    ),   
    /* Alarm Timeout descriptor */
    BT_GATT_CUD("Alarm Timeout", BT_GATT_PERM_READ),

    /* Alarm Clear characteristic, handle 15 */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_DS_ALC),     
                            BT_GATT_CHRC_WRITE | 
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                            BT_GATT_PERM_WRITE,
                            NULL, on_receive,
                            &alc_value
    ),   
    /* Alarm Clear descriptor */
    BT_GATT_CUD("Alarm Clear", BT_GATT_PERM_READ),

);



