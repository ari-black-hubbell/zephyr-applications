
/* ble_cus.c */

/* module imports */ 
// standard c
#include <string.h>
// local files
#include "ble_cus.h"
// zephyr library
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>

/* define custom service init function */
uint32_t ble_cus_init(ble_cus_t* p_cus, const ble_cus_init_t* p_cus_init) {
    // check that no passed-in pointers are NULL
    if (p_cus == NULL || p_cus_init == NULL) { return NRF_ERROR_NULL; }

    // declare error code and UUID variables
    uint32_t    err_code;
    ble_uuid_t  ble_uuid;

    // initialize custom service structure
    // set the connection handle to invalid (valid only when we're in a connection)
    p_cus->conn_handle = BLE_CONN_HANDLE_INVALID;

    // add custom base UUID to BLE stack's table
    ble_uuid128_t base_uuid = {CUSTOM_SERVICE_UUID_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_cus->uuid_type);
    VERIFY_SUCCESS(err_code);

    // set custom BLE UUID's attributes (type and UUID)
    ble_uuid.type = p_cus->uuid_type;
    ble_uuid.uuid = CUSTOM_SERVICE_UUID;

    // add the custom service declaration to the BLE stack's GATT table
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_cus->service_handle);
    if (err_code != NRF_SUCCESS) { return err_code; }
}

