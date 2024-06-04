
/* main.c */

// note: needed to edit some paths in order to work with newest version of zephyr
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

#include <soc.h>
#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "../services/my_service.h"


/* definitions */
#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME           // device name set in proj.conf.
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)       // the length of the device name.

/* initialize a semaphore (ble_init_ok) with initial value 0 and max value 1.
 * forced mutex; value is 1 if resource is free, 0 otherwise
*/
static K_SEM_DEFINE(ble_init_ok, 0, 1);

/* our bt_conn (bluetooth connection) pointer. */
struct bt_conn *my_connection;

/* check if the connection is valid. */
static void connected(struct bt_conn *conn, uint8_t err) {      // changed from u8_t (incompatiblity)
        // structure to hold connection information
        struct bt_conn_info info;
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
static void disconnected(struct bt_conn *conn, uint8_t reason) {
        printk("Disconnected (reason %u)\n", reason);
        my_connection = NULL;
}


/* check that the supplied parameters are valid. */
static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param) {
        return true;
}

/* update connection parameters. */
static void le_param_updated(struct bt_conn *conn, uint16_t interval, uint16_t latency, uint16_t timeout) {
        // structure to hold connection information
        struct bt_conn_info info;
        // char array to hold bt address
        char addr[BT_ADDR_LE_STR_LEN];
        // check if connection information is valid
        if (bt_conn_get_info(conn, &info)) { printk("Could not parse connection info\n"); }
        // display updated parameter information
        else {
                bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));   // need destination, address, and size of address

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

/* advertisement data structure */
static const struct bt_data ad[] = {
        // set advertising flags
        BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),   // advertise indefinitely | don't support BT classic
        // define device name
        BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),           // name in config file
};

/* scan response data structure */
static const struct bt_data sd[] = {
        // send service UUID
        BT_DATA_BYTES(BT_DATA_UUID16_ALL, MY_SERVICE_UUID)
};

/* a callback that contains code we run after the bluetooth host is enabled. */
static void bt_ready(int err) {
        if (err) {
                printk("BLE init failed with error code %d\n", err);
                return;
        }
        // configure connection callbacks
        bt_conn_cb_register(&conn_callbacks);
        // initialize services 
        err = my_service_init();
        // raise error
        if (err) { 
                printk("Failed to init LBS (err:%d)\n", err);
                return;
        }
        // start advertising
        // err = bt_le_adv_start(BT_LE_ADV_PARAM(
        //                                 BT_LE_ADV_OPT_CONNECTABLE       // options (connectable)
        //                                 | BT_LE_ADV_OPT_ONE_TIME        // options (advertise once)
        //                                 | BT_LE_ADV_OPT_USE_NAME,       // options (use GAP device name)
        //                                 160,                            // min advertising interval (units of 0.625 ms)
        //                                 1600),                          // max advertising interval(units of 0.625 ms)
        //                         ad, ARRAY_SIZE(ad),     // ad (data in ad packets) and size
        //                         sd, ARRAY_SIZE(sd));    // sd (data in scan response packets) and size
	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
        
        // raise error
        if (err) {
                printk("Advertising failed to start (err%d)\n", err);
                return;
        }
        // display success message
        printk("Advertising successfully started\n");

        // give a semaphore to increment its count
        k_sem_give(&ble_init_ok);


}

/* raise an error and sleep the thread indefinitely. */
static void error(void) {
        while (true) {
                printk("Error!\n");     // note: can log this to console instead
                // spin indefinitely
                k_sleep(K_MSEC(1000));  // 1000ms timeout
        }
}






/* main function, initialize BLE */
int main(void) {
        // init error code and pulse
        int err = 0;
        uint32_t number = 0;
        printk("Starting Nordic BLE peripheral tutorial\n");
        // enable the bluetooth host stack
        err = bt_enable(bt_ready);   
        // raise error
        if (err) {
                printk("BLE initialization failed\n");
                error();
        }

	/* Bluetooth stack should be ready in less than 100 msec.
	 * We use this semaphore to wait for bt_enable to call bt_ready before we proceed
	 * to the main loop. By using the semaphore to block execution we allow the RTOS to 
	 * execute other tasks while we wait. 
        */
       
        // give a semaphore to increment its count
        err = k_sem_take(&ble_init_ok, K_MSEC(500));
        // raise error
        if (!err) { printk("Bluetooth initialized\n"); }
        else {
                printk("BLE initialization did not complete in time\n");
                error();        // catch error with defined method
        }
        // initialize services
        err = my_service_init();
        // main loop to send an incrementing number (pulse) every 2000ms
        for (;;) {
                my_service_send(my_connection, (uint8_t *)&number, sizeof(number));     // send the number via connection
                number++;                                                               // increment the number
                k_sleep(K_MSEC(1000));                                                  // 1000 ms

        }

        return 0;
}
