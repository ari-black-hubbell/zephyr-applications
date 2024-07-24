
/* wireless_security.c 
 * A custom service consisting of the following characteristics:
 *    Initialization Vector IV (read only), Passkey (write only).  
 */


#include "services.h"

/* (Passkey) 32 byte array to initialize the HMAC computation for the passkey.
 * used to add randomness, read-only.
 */
uint8_t iv[32] = {0};

/* (Initialization Vector) value used to authenticate connecting devices.
 * calculated using the IV, then sent for verification, write-only.
 */
uint8_t passkey[32] = {0};
// __uint128_t passkey = 0x00;
// uint

void hmac(void) {
    // printk("in hmac");

    struct hash_ctx ctx;
    ctx.flags = CAP_SYNC_OPS | CAP_SEPARATE_IO_BUFS;
    const struct device *dev = device_get_binding(CONFIG_CRYPTO_MBEDTLS_SHIM_DRV_NAME);

    int ret;
    ret = hash_begin_session(dev, &ctx, CRYPTO_HASH_ALGO_SHA256);
    if (ret == 0) {printk("initialized sha256 session\n");}
    else {printk("failed to initialize sha256 session\n");}

    char hash_val[] = {0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t out_buf[32] = {0};
    struct hash_pkt pkt = {
        .in_buf = hash_val,
        .in_len = sizeof(hash_val),
        .out_buf = out_buf,
    };

    ret = hash_compute(&ctx, &pkt);
    if (ret == 0) {printk("successfully computed hash\n");}
    else {printk("failed to compute hash\n");}

    // char* cur = pkt.out_buf;
    // for ( ; *cur != '\0'; ++cur) {
    //     printk("%02x", *cur);
    // }
    // printk("\n");

    // for smaller 1 byte output
    // uint8_t output[] = {
    //     0x6e, 0x34, 0x0b, 0x9c, 0xff, 0xb3,
    //     0x7a, 0x98, 0x9c, 0xa5, 0x44, 0xe6,
    //     0xbb, 0x78, 0x0a, 0x2c, 0x78, 0x90,
    //     0x1d, 0x3f, 0xb3, 0x37, 0x38, 0x76,
    //     0x85, 0x11, 0xa3, 0x06, 0x17, 0xaf, 
    //     0xa0, 0x1d,
    // };

    // for larger 16 byte output
    uint8_t output[] = {
        0x37, 0x47, 0x08, 0xff, 0xf7, 0x71, 0x9d, 0xd5,
        0x97, 0x9e, 0xc8, 0x75, 0xd5, 0x6c, 0xd2, 0x28,
        0x6f, 0x6d, 0x3c, 0xf7, 0xec, 0x31, 0x7a, 0x3b, 
        0x25, 0x63, 0x2a, 0xab, 0x28, 0xec, 0x37, 0xbb,
    };

    // new: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb
    // app: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb


    printk("expected hash start: %d\n", output[0]);
    printk("actual hash start: %d\n", pkt.out_buf[0]);

    for (int i = 0; i < 32; i++) {
        if (output[i] == pkt.out_buf[i]) {
            /* write to passkey characteristic */
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

    









    /* demonstrate that a hash can be computed on both sides, proof of concept.
     * show that it's possible/feasible to really make the whole thing.
     */

    /* output hash (with 4 extra characters at the end ?)*/





    // take 2: 0x00 hash
    // test app: 
    // 6e340b9cffb37a989ca544e6bb780a2c78901d3fb33738768511a30617afa01d
    // zephyr:
    // 6e340b9cffb37a989ca544e6bb780a2c78901d3fb33738768511a30617afa01d a5c5
    // emulator:
    // ffb37a989ca544e6bb780a2c78901d3fb33738768511a30617afa01d16

    /* K: authentication key, shared secret */

    /* K0: K | 0x00 (32 times) */
    // uint8_t k0[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    //                 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    //                 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    //                 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    // /* address: temporarily hard coded for ease of use */
    // // 60:B6:47:1E:EF:E5
    // uint8_t addr[] = {0xE5, 0xEF, 0x1E, 0x47, 0xB6, 0x60};

    // /* text: IV || BLE address */
  
    // printk("size of iv: %d\n", sizeof(iv));
    // printk("size of addr: %d\n", sizeof(addr));

    // uint8_t text[sizeof(iv) + sizeof(addr)];
    // for (int i=0; i<sizeof(iv); i++) {
    //     text[i] = iv[i];
    // }
    // for (int i=0; i<sizeof(addr); i++) {
    //     text[i+sizeof(iv)] = addr[i];
    // }

    // printk("new size of text: %d\n", sizeof(text)); // should be 38

    // /* opad: byte 0x5C repeated 64 times */
    // uint8_t opad[64];
    // for (int i=0; i<64; i++) {
    //     opad[i] = 0x5C;
    // }

    // /* ipad: byte 0x36 repeated 64 times */
    // uint8_t ipad[64];
    // for (int i=0; i<64; i++) {
    //     ipad[i] = 0x5C;
    // }



}


/* function called whenever RX characteristic is read by a client. 
 * unpacks and outputs requested records from the database.
 */
static ssize_t on_read(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf,
			    uint16_t len, uint16_t offset) {

        hmac();


    
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &iv, sizeof(iv));
}

static ssize_t on_read_passkey(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf,
			    uint16_t len, uint16_t offset) {
    
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &passkey, sizeof(passkey));
}

/* function called whenever characteristic is written to by a client. 
 * unpacks and outputs received data.
 */
static ssize_t on_receive(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf,
                uint16_t len, uint16_t offset, uint8_t flags) {

    // const uint8_t * buffer = buf;
    // printk("Received data, handle %d, conn %p, data 0x", attr->handle, conn);
    // for(uint8_t i = 0; i < len; i++) { printk("%02X", buffer[i]);}              // iterate through buffer, print all data
    // printk("\n");
    // return len;
        /* do not accept inputs too large (4 bytes / 2 words) */
    if (len > 8) { 
        printk("(ERR) input value too large,\n maximum length is 16 bytes\n");
        printk("===============================\n");
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN); 
    }

    /* clear passkey */
    // passkey = {0};
    memset(passkey, 0, sizeof(passkey));

    /* update the value */
    uint8_t *value = attr->user_data;
    // value = 0x00;
    memcpy(value, buf, len);                        /* is this safe? */
    for(uint8_t i = 0; i < len; i++) {
        printk("%X", value[i]);
        passkey[i] = value[i];                      /* copy over the byte */
        }
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
    // BT_GATT_CHARACTERISTIC( BT_UUID_DECLARE_128(BT_UUID_WLS_IV),    /* characteristic UUID */  
    //                         BT_GATT_CHRC_WRITE |                    /* properties */
    //                         BT_GATT_CHRC_WRITE_WITHOUT_RESP | BT_GATT_CHRC_READ,                      
    //                         BT_GATT_PERM_WRITE | BT_GATT_PERM_READ,                     /* permissions */
    //                         on_read, on_receive,                       /* callbacks (read, write) */
    //                         &passkey                                /* user data */
    // ),
    /* Passkey descriptor */
    BT_GATT_CUD("Passkey", BT_GATT_PERM_READ),                      /* read only */
);