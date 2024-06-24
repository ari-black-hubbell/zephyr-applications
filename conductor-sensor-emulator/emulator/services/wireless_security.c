
/* wireless_security.c 
 * A custom service consisting of the following characteristics:
 *    Initialization Vector IV (read only), Passkey (write only).  
 */


#include "services.h"


/* function called whenever RX characteristic is read by a client. 
 * unpacks and outputs requested records from the database.
 */
static ssize_t on_read(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset) {
    /* TODO: read value(s) from database */
	uint8_t IV = 0;    /* temporary value */
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &IV,
				 sizeof(IV));
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


/* A macro to statically register the service in the host stack. 
 */
BT_GATT_SERVICE_DEFINE(WLS,                                          /* service name */
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_128(BT_UUID_WLS)),       /* service UUID */ 

    /* IV characteristic */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_WLS_IV),    /* characteristic UUID */  
                            BT_GATT_CHRC_READ,                      /* properties */
                            BT_GATT_PERM_READ,                      /* permissions */
                            on_read, NULL,                          /* callbacks (read, write) */
                            NULL                                    /* user data */
    ),
    /* IV descriptor */
    BT_GATT_CUD("Initialization Vector", BT_GATT_PERM_READ),        /* read only */

    /* Passkey characteristic */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_WLS_IV),    /* characteristic UUID */  
                            BT_GATT_CHRC_WRITE |                    /* properties */
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP,                      
                            BT_GATT_PERM_WRITE,                     /* permissions */
                            NULL, on_receive,                       /* callbacks (read, write) */
                            NULL                                    /* user data */
    ),
    /* Passkey descriptor */
    BT_GATT_CUD("Passkey", BT_GATT_PERM_READ),                      /* read only */
);