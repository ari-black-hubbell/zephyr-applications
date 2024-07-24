
/* wireless_security.c 
 * A custom service consisting of the following characteristics:
 *    Initialization Vector IV (read only), Passkey (write only).  
 */


#include "services.h"

/* (Passkey) 32 byte array to initialize the HMAC computation for the passkey.
 * used to add randomness, read-only.
 */
uint8_t iv[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* (Initialization Vector) value used to authenticate connecting devices.
 * calculated using the IV, then sent for verification, write-only.
 */
uint8_t passkey[32] = {0};

/* Perform an HMAC using IV to calculate the passkey to be shared with the connected device.
 *
 * Currently: function just uses default 32-byte 0x00 array as IV to calculate hash, and sets the received
 *            value as the passkey. 
 * 
 * TODO: Perform HMAC as specified in requirements and set resultant value as the passkey.
 */
void hmac(void) {

    /* context structure  */
    struct hash_ctx ctx;

    /* context flags */
    ctx.flags = CAP_SYNC_OPS | CAP_SEPARATE_IO_BUFS;

    /* device structure */
    const struct device *dev = device_get_binding(CONFIG_CRYPTO_MBEDTLS_SHIM_DRV_NAME);

    /* variable to hold return/error code */
    int ret;

    /* begin hash session  */
    ret = hash_begin_session(dev, &ctx, CRYPTO_HASH_ALGO_SHA256);
    if (ret == 0) {printk("initialized sha256 session\n");}
    else {printk("failed to initialize sha256 session\n");}

    // char hash_val[] = {0x00, 0x00, 0x00, 0x00, 0x00,
    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    /* (cleared) buffer to hold hash output */
    uint8_t out_buf[32] = {0};
    struct hash_pkt pkt = {
        .in_buf = hash_val,
        .in_len = sizeof(hash_val),
        .out_buf = out_buf,
    };

    /* compute hash */
    ret = hash_compute(&ctx, &pkt);
    if (ret == 0) {printk("successfully computed hash\n");}
    else {printk("failed to compute hash\n");}


    /* expected output from default hash: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb */
    uint8_t output[] = {
        0x37, 0x47, 0x08, 0xff, 0xf7, 0x71, 0x9d, 0xd5,
        0x97, 0x9e, 0xc8, 0x75, 0xd5, 0x6c, 0xd2, 0x28,
        0x6f, 0x6d, 0x3c, 0xf7, 0xec, 0x31, 0x7a, 0x3b, 
        0x25, 0x63, 0x2a, 0xab, 0x28, 0xec, 0x37, 0xbb,
    };

    /* output hash result and write to passkey characteristic */
    printk("expected hash start: %d\n", output[0]);
    printk("actual hash start: %d\n", pkt.out_buf[0]);
    for (int i = 0; i < 32; i++) {
        if (output[i] == pkt.out_buf[i]) {
            passkey[i] = pkt.out_buf[i];
            printk("passkey val: %d ", passkey[i]);
            continue;
        }
        else {
            printk("found incorrect hash value. expected %d at index %d, got %d\n", output[i], i, pkt.out_buf[i]);
        }
    }
    printk("check completed :)\n");
    printk("passkey successfully written\n");
    printk("final passkey val: %d\n", passkey[31]);

}


/* Callback: called whenever IV characteristic is read by a client. 
 * Unpacks and outputs the requested value.
 */
static ssize_t on_read(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf,
			    uint16_t len, uint16_t offset) {
        hmac();
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &iv, sizeof(iv));
}

/* Callback: called whenever passkey characteristic is read by a client.
 * Unpacks and outputs the requested value.
 */
static ssize_t on_read_passkey(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf,
			    uint16_t len, uint16_t offset) {
    
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &passkey, sizeof(passkey));
}

/* Callback:  called whenever a characteristic is written to by a client. 
 * unpacks and outputs received data.
 */
static ssize_t on_receive(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf,
                uint16_t len, uint16_t offset, uint8_t flags) {

        /* do not accept inputs too large (32 bytes / 16 words) */
    if (len > 16) { 
        printk("(ERR) input value too large,\n maximum length is 32 bytes\n");
        printk("===============================\n");
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN); 
    }

    /* clear the passkey characteristic */
    memset(passkey, 0, sizeof(passkey));

    /* update the value */
    uint8_t *value = attr->user_data;

    memcpy(value, buf, len);                        /* is this safe? */
    for(uint8_t i = 0; i < len; i++) {
        printk("%X", value[i]);
        passkey[i] = value[i];                      /* copy over the byte */
        }

    /* output the new value to console */
    char arr[sizeof(passkey)];
    printk("\npasskey: ");
    bt_gatt_attr_read(conn, attr, arr, len, offset, &passkey, sizeof(passkey));
    for(uint8_t i = 0; i < len; i++) {printk("%X", arr[i]);}
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
    // BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_WLS_PK),    /* characteristic UUID */  
    //                         BT_GATT_CHRC_WRITE |                    /* properties */
    //                         BT_GATT_CHRC_WRITE_WITHOUT_RESP,                      
    //                         BT_GATT_PERM_WRITE,                     /* permissions */
    //                         NULL, on_receive,                       /* callbacks (read, write) */
    //                         &passkey                                /* user data */
    // ),
    /* utility; meant to be write only, but allowing reads makes easier verification */
    BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_WLS_PK),    /* characteristic UUID */  
                            BT_GATT_CHRC_WRITE |                    /* properties */
                            BT_GATT_CHRC_WRITE_WITHOUT_RESP | BT_GATT_CHRC_READ,                      
                            BT_GATT_PERM_WRITE | BT_GATT_PERM_READ,                     /* permissions */
                            on_read_passkey, on_receive,                       /* callbacks (read, write) */
                            &passkey                                /* user data */
    ),

    /* Passkey descriptor */
    BT_GATT_CUD("Passkey", BT_GATT_PERM_READ),                      /* read only */
);