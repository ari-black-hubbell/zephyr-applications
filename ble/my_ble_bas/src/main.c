#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/services/bas.h>


// register the logging module (so we can use logging instead of printk)
LOG_MODULE_REGISTER(ble_test);

// define bluetooth ready flag/boolean
volatile bool ble_ready = false;

// define structure for advertising
static const struct bt_data ad[] = {    // args: type, data_len, *data
        BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),   // flags: advertising available for long period; using BLE, classic BR/EDR not supported
        BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_BAS_VAL))   // data is the 16-bit UUID from BAS
};

// check if bluetooth is ready for use
void bt_ready(int err) {
        if (err) {
                // log any BLE error on check
                LOG_ERR("bt enable return %d", err);
        }
        LOG_INF("bt_ready!");
        ble_ready = true;

}

// init bluetooth low energy
int init_ble(void) {
        // add to the log
        LOG_INF("Init BLE");
        int err;
        // enable bluetooth
        err = bt_enable(bt_ready);
        // log any BLE error on initialization
        if (err) {
                LOG_ERR("bt_enable failed (err %d)", err);
                return err;
        }
        return 0;
}

int main(void) {

        // initializations
        init_ble();
        while(!ble_ready) { // check ble_ready flag for status
                LOG_INF("BLE stack not ready yet");
                k_msleep(100);
        }

        // when bluetooth ready / inits complete
        LOG_INF("BLE stack ready");

        // advertise services offered when bluetooth is running
        int err;
        err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0); // send empty advertisement packet 
        if (err) {
                printk("Advertising failed to start (err %d)\n", err);
                return 1;
        }



        return 0;
}


