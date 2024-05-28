
/*  main.c  */


/* modules */
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/services/bas.h>

/* define base UUIDs */
#define CUSTOM_BASE_UUID_w32    0x12345678
#define CUSTOM_BASE_UUID_w1     0x90AB
#define CUSTOM_BASE_UUID_w2     0xCDEF
#define CUSTOM_BASE_UUID_w3     0x0123
#define CUSTOM_BASE_UUID_w48    0x456789ABCDEF

/* define UUIDs for services and characterisics */
#define CUSTOM_SERVICE_UUID             0x1
#define CUSTOM_CHARACTERISTIC_UUID      0x2
#define BT_UUID_CUSTOM_SERVICE          BT_UUID_128_ENCODE(     // combine the base UUIDs to create actual UUIDs
        CUSTOM_BASE_UUID_w32, CUSTOM_BASE_UUID_w1, CUSTOM_BASE_UUID_w2, CUSTOM_BASE_UUID_w3,
        CUSTOM_BASE_UUID_w48 + CUSTOM_SERVICE_UUID)
#define BT_UUID_CUSTOM_CHARACTERISTIC   BT_UUID_128_ENCODE(
        CUSTOM_BASE_UUID_w32, CUSTOM_BASE_UUID_w1, CUSTOM_BASE_UUID_w2, CUSTOM_BASE_UUID_w3,
        CUSTOM_BASE_UUID_w48 + CUSTOM_CHARACTERISTIC_UUID)


/* initialize data structures for UUIDs */
static struct bt_uuid_128 custom_service_uuid = BT_UUID_INIT_128(BT_UUID_CUSTOM_SERVICE);
static struct bt_uuid_128 custom_characteristic_uuid = BT_UUID_INIT_128(BT_UUID_CUSTOM_CHARACTERISTIC);

/* create custom service with custom characteristic */
BT_GATT_SERVICE_DEFINE(
        custom_service,                                                 // service name
        BT_GATT_PRIMARY_SERVICE(&custom_service_uuid),                  // assigns (primary) service the custom uuid
        BT_GATT_CHARACTERISTIC(&custom_characteristic_uuid.uuid,        // assigns characteristic the custom uuid
                BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,                // allow read/notify for remote devices
                BT_GATT_PERM_READ,                                      // set read permission
                custom_data_callback, NULL, custom_data),               // call function during read, notify with custom data
        BT_GATT_CCC(nextiles_motion_notify_cb, BT_GATT_PERM_READ | BT_GET_PERM_WRITE)
        );

/* respond to characteristic reads/notifications */
// #define VND_MAX_LEN 20
static uint8_t custom_data[DATA_LEN];
static ssize_t custom_data_callback(
        struct bt_conn *conn, const struct bt_gatt_attr *attr, 
        void *buf, uint16_t len, uint16_t offset) {
                return bt_gatt_attr_read(conn, attr, buf, len, offset, custom_data, DATA_LEN);
        }
void some_thread(void *p1, void *p2, void *p3) {
        while (1) {
                struct bt_gatt_attr *custom_char = bt_gatt_find_by_uuid(attr_custom_bt_service, 0, &custom_characteristic_uuid);
                bt_gatt_notify(NULL, custom_chat, custom_data, sizeof(otion_data));
                k_msleep(MSEC(1000));
        }
}