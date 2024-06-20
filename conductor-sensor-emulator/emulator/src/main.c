
/* main.c */

#include <zephyr/kernel.h>
#include <zephyr/console/console.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "../services/services.h"



#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME           
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)

/* initialize semaphore */
static K_SEM_DEFINE(ble_init_ok, 0, 1); /* semaphore as forced mutex, 1 if avail 0 if not. */

/* instantiate bluetooth connection */
struct bt_conn *my_connection;


/* Output a greeting message to the console.

 * Returns: (int) 0 on success, != 0 on error.
 */
int greet(void) {
    
    /* write to console. */ 
    printk("\nConductor Sensor Emulator\n");

    return 0;

}

/* Callback: notify the application of a new connection.
 *
 * `struct bt_conn *conn`: The new BT connection object.
 * `uint8_t err`:          HCI error, 0 for success and != 0 otherwise.
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

/* Callback: notify the application of a connection termination.
 *
 * `struct bt_conn *conn`: The new BT connection object.
 * `uint8_t reason`:       Reason for disconnect, `BT_HCI_ERR_*`.
 */
static void disconnected(struct bt_conn *conn, uint8_t reason) {
    printk("Disconnected (reason %u)\n", reason);
    my_connection = NULL;
}

/* Callback: notify the application of parameter update request.
 *
 * `struct bt_conn *conn`:           The new BT connection object.
 * `struct be_le_conn_param *param`: The proposed connection parameters.
 * 
 * Returns: (`true`).
 */
static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param) {
    /* accept all parameter change requests */
    return true;
}

/* Callback: notify the application that the parameters have been updated.
 *
 * `struct bt_conn *conn`:           The BT connection object.
 * `uint16_t interval`:              The new connection interval.
 * `uint16_t latency`:               The new connection latency.
 * `uint16_t timeout`:               The new connection supervision timeout.
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

/* (1st) advertisement packet data structure */
static const struct bt_data ad[] = {
    
    /* set advertising flags */
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)), /* advertise indefinitely, no BT classic. */
    
    /* advertise some data */
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)          /* our full device name. */

};

/* (2nd) advertisement packet data structure */
// static const struct bt_data sd[] = {
    
//     /* advertise some data */
//     BT_DATA_BYTES(BT_DATA_UUID16_ALL, &val)  /* the UUID of one of our services. */

// };



int main(void) {

    /* initialize console */
    console_init();                         /* initialize CLI. */

    /* instantiate local variables */
    int ret;                                /* to hold return codes. */




    /* greet */
    greet();
    
    return 0;
}

