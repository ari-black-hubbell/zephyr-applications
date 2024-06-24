
/* sensor_info.c 
 * A custom service consisting of the following read-only characteristic:
 *    Conductor Sensor Health Data.  
 */


#include "services.h"


/* function called whenever RX characteristic is read by a client. 
 * unpacks and outputs requested records from the database.
 */
static ssize_t on_read(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset) {
    /* TODO: read value(s) from database */
	uint8_t health_data = 0;    /* temporary value */
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &health_data,
				 sizeof(health_data));
}


/* A macro to statically register the service in the host stack. 
 */
BT_GATT_SERVICE_DEFINE(SI,                                          /* service name */
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_128(BT_UUID_SI)),       /* service UUID */ 

    /* Sensor Health Data characteristic */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_SI_SHD),    /* characteristic UUID */  
                            BT_GATT_CHRC_READ,                       /* properties */
                            BT_GATT_PERM_READ,                      /* permissions */
                            on_read, NULL,                          /* callbacks (read, write) */
                            NULL                                    /* user data */
    ),
    /* Sensor Health Data descriptor */
    BT_GATT_CUD("Sensor Health Data", BT_GATT_PERM_READ),           /* read only */
);