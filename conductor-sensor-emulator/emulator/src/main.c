
/* main.c */

#include <zephyr/kernel.h>
#include <zephyr/console/console.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/bluetooth/services/dis.h>


#include "../services/services.h"



#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME           
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)
#define COMPANY_ID              CONFIG_BT_COMPANY_ID
#define COMPANY_ID_LEN          (sizeof(COMPANY_ID) - 1)

/* initialize semaphore */
static K_SEM_DEFINE(ble_init_ok, 0, 1); /* semaphore as forced mutex, 1 if avail 0 if not. */

/* instantiate bluetooth connection */
struct bt_conn *my_connection;


/* Output a greeting message to the console.

 * @return 0 on success, != 0 on error.
 */
int greet(void) {
    
    /* write to console. */ 
    printk("\nConductor Sensor Emulator\n");

    return 0;

}

/* Connection callback: notify the application of a new connection.
 *
 * @param *conn     The new BT connection object.
 * @param err       HCI error, 0 for success and != 0 otherwise.
 */
static void connected(struct bt_conn *conn, uint8_t err) {
    
    /* initialize info struct */
    struct bt_conn_info info;               /* contains information about the specified BT connection. */
    char addr[BT_ADDR_LE_STR_LEN];          /* the BLE address char array. */

    /* display connection information  */
    if (bt_conn_get_info(conn, &info)) { printk("Could not parse connection info\n"); }
    else {
        bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));   /* convert dst addr to string. */

        /* format output */
        printk("Connection parameters updated!  \n\
        Connected to: %s                        \n\
        New connection interval: %u             \n\
        New peripheral latency: %u              \n\
        New connection supervisory timeout: %u  \n"
        , addr, info.le.interval, info.le.latency, info.le.timeout);
    }

}

/* Connection callback: notify the application of a connection termination.
 *
 * @param *conn    The new BT connection object.
 * @param reason   Reason for disconnect, `BT_HCI_ERR_*`.
 */
static void disconnected(struct bt_conn *conn, uint8_t reason) {
    printk("Disconnected (reason %u)\n", reason);
    my_connection = NULL;
}

/* Connection callback: notify the application of parameter update request.
 *
 * @param *conn     The new BT connection object.
 * @param *param    The proposed connection parameters.
 * 
 * @return `true`
 */
static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param) {
    /* accept all parameter change requests */
    return true;
}

/* Connection callback: notify the application that the parameters have been updated.
 *
 * @param *conn         The BT connection object.
 * @param interval      The new connection interval.
 * @param latency       The new connection latency.
 * @param timeout       The new connection supervision timeout.
 */
static void le_param_updated(struct bt_conn *conn, uint16_t interval, uint16_t latency,
                             uint16_t timeout) {
    /* initialize info struct */
    struct bt_conn_info info;       /* contains information about the specified BT connection. */
    char addr[BT_ADDR_LE_STR_LEN];  /* the BLE address char array. */

    /* display connection information  */
    if (bt_conn_get_info(conn, &info)) { printk("Could not parse connection info\n"); }
    else {
        bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));   /* convert dst addr to string. */

        /* format output */
        printk("Connection parameters updated!  \n\
        Connected to: %s                        \n\
        New connection interval: %u             \n\
        New peripheral latency: %u              \n\
        New connection supervisory timeout: %u  \n"
        , addr, info.le.interval, info.le.latency, info.le.timeout);
    }
}

/* BLE connection status callback structure */
static struct bt_conn_cb conn_callbacks = {
        .connected              = connected,
        .disconnected           = disconnected,
        .le_param_req           = le_param_req,
        .le_param_updated       = le_param_updated
};

#define BT_CUSTOM_DATA 0x08, 0xA0  // containing custom data

/* (1st) advertisement packet data structure */
static const struct bt_data ad[] = {
    
    /* set advertising flags */
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)), /* advertise indefinitely, no BT classic. */
    // BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
    


    // proposed format: flags, manuf. data, name
    // manuf. data:
        // company ID (0, 1)
        // product ID (2, 3)
        // message format (4)
        // mtu ID (5, 6, 7, 8)
        // rssi (?)
        // peripheral name




    BT_DATA_BYTES(BT_DATA_MANUFACTURER_DATA, 
            0xA0, 0x08,      // company ID
            0x03, 0x00,     // product ID
            0x01,           // message format
            0x00, 0x00,
            0x00, 0x00      // mtu ID
                            // rssi
        
        
        
        ),


    // BT_DATA_BYTES(BT_CUSTOM_DATA, 0xA0, 0x08),
    
    /* advertise some data */
    // BT_DATA(BT_DATA_MANUFACTURER_DATA, COMPANY_ID, sizeof(COMPANY_ID) - 1)
    // BT_DATA_BYTES(BT_DATA_MANUFACTURER_DATA, 0x08, 0xA0),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)          /* our full device name. */
    // BT_DATA(BT_DATA_NAME_SHORTENED, DEVICE_NAME, 8)
    // BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1)

};


/* (2nd) advertisement packet data structure */
static const struct bt_data sd[] = {

    /* advertise some data */

    // BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_SI)   /* the UUID of our primary service. does it matter what we choose? */  
    // BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1) 
    // BT_DATA(BT_DATA_MANUFACTURER_DATA, COMPANY_ID, COMPANY_ID_LEN)
    // BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_DIS_VAL))  
    // BT_DATA(BT_DATA_MANUFACTURER_DATA)
    // 0x08A0

};

/* Initialize services.
 *
 * @return      0 on success, != 0 otherwise.
 */
int init_services(void) {
    
    int err = 0;                            /* return (error) value. */

    uint8_t data[MAX_TRANSMIT_SIZE];        /* transmission buffer to hold data. */

    memset(&data, 0, MAX_TRANSMIT_SIZE);

    return err;
}


/* BT Callback: start advertising after the bluetooth host stack is enabled.
 *
 * @param err   HCI error, 0 for success and != 0 otherwise.
 */
static void bt_ready(int err) {

    if (err) { printk("BLE initialization failed with error code %d\n", err); return; }

    /* configure connection callbacks */
    bt_conn_cb_register(&conn_callbacks);

    /* initialize services */
    err = init_services();
    if (err) { printk("Failed to initialize services, error code %d\n", err); return; } 

    /* start advertising */
    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err) { printk("Failed to start advertising, error code %d\n", err); return; }

    /* mutex */
    k_sem_give(&ble_init_ok);   /* increment semaphore count to 1. */
}

/* BT Callback: raise an error and spin indefinitely.
 */
static void error(void) {
    while (true) {
        printk("Error!\n");
        k_sleep(K_MSEC(1000));  /* 1000ms timeout */
    }
}




int main(void) {

    /* initialize console */
    console_init();                         /* initialize CLI. */

    /* greet */
    greet();

    /* instantiate local variables */
    int err;                                /* to hold return codes. */

    /* enable the bluetooth host stack */
    err = bt_enable(bt_ready);
    if (err) { printk("BLE initialization failed\n"); error(); }

    /* use a semaphore to wait for bt_enable to call bt_ready, frees RTOS to complete other tasks */
    err = k_sem_take(&ble_init_ok, K_MSEC(500));    /* increment semaphore count to 1 (mutex). */
    if (err) { printk("BLE initializtion did not complete in time\n"); error(); }

    /* initialize services */
    err = init_services();

    /* TODO: call service methods from their respective files here. */
    
    return 0;
}

