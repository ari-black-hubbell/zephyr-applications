
/* main.c */

// note: needed to edit some paths in order to work with newest version of zephyr
#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/drivers/gpio.h>
#include <soc.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "../services/my_service.h"


/* definitions */
#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME           // device name set in proj.conf
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)       

/* initialize a semaphore (ble_init_ok) with initial value 0 and max value 1.
 * forced mutex; value is 1 if resource is free, 0 otherwise
*/
static K_SEM_DEFINE(ble_init_ok, 0, 1);

/* our bt_conn (bluetooth connection) pointer. */
struct bt_conn *my_connection;

/* check if the connection is valid. */
static void connected(struct bt_conn *conn, u8_t err) {
        // structure to hold connection information
        struct b_t_conn_info info;
        // char array to hold bt address
        char addr[BT_ADDR_LE_STR_LEN];
        // check if connection failed
        if (err) { printk("Connection failed (err %u)\n", err); return; }
        // check if connection information is valid
        else if (bt_conn_get_info(conn, &info)) { printk("Could not parse connection info\n"); }
        // display connection information
        else {
                bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));   // need destination, address, and size of address

                printk("Connection established!         \n\
                Connected to: %s                        \n\
                Role: %u                                \n\
                Connection interval: %u                 \n\
                Peripheral latency: %u                  \n\
                Connection supervisory timeout: %u      \n"     // note: used term "peripheral" instead of "slave" (...)
                , addr, info.role, info.le.interval, info.le.latency, info.le.timeout);
        }
}

/* reset the connection pointer once disconnected. 
 * returns true if parameters are acceptable, and returns false otherwise.
*/
static void disconnected(struct bt_conn *conn, u8_t reason) {
        printk("Disconnected (reason %u)\n", reason);
        my_connection = NULL;
}


/* check that the supplied parameters are valid. */
static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param param) {
        return true;
}

/* update connection parameters. */
static void le_param_updated(struct bt_conn *conn, u16_t interval, u16_t latency, u16_t timeout) {
        // structure to hold connection information
        struct b_t_conn_info info;
        // char array to hold bt address
        char addr[BT_ADDR_LE_STR_LEN];
        // check if connection information is valid
        else if (bt_conn_get_info(conn, &info)) { printk("Could not parse connection info\n"); }
        // display updated parameter information
        else {
                bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));   // need destination, address, and size of address

                printk("Connection parameters updated!  \n\
                Connected to: %s                        \n\
                New connection interval: %u             \n\
                New peripheral latency: %u              \n\
                New connection supervisory timeout: %u  \n"
                , addr, info.le.interval, info.le.legacy, info.le.timeout);
        }
}

/* BLE connection status callback structure */
static struct bt_conn_cb conn_callbacks = {
        .connected              = connected,
        .disconnected           = disconnected,
        .le_param_req           = le_param_req,
        .le_param_pdated        = le_param_updated
};

/* a callback that contains code we run after the bluetooth host is enabled. */
static void bt_ready(int err) {
        if (err) {
                printk("BLE init failed with error code %d\n", err);
                return;
        }
        // give a semaphore to increment its count
        k_sem_give(&ble_init_ok);
        // configure connection callbacks
        bt_conn_cb_register(&conn_callbacks);
}

/* raise an error and sleep the thread indefinitely. */
static void error(void) {
        while (true) {
                printk("Error!\n");     // note: can log this to console instead
                // spin indefinitely
                k_sleep(K_MSEC(1000));  // 1000ms timeout
        }
}






/* < TODO: add function description here > */
int main(void) {
        // enable the bluetooth host stack
        err = bt_enable(bt_ready);    





        return 0;
}
