
/* ble_cus.h */

/* module imports */ 
#include <stdint.h>
#include <stdbool.h>

/* select a 128-bit UUID (any value) */
// website: https://www.uuidgenerator.net/version4
// f8cab862-55ff-4069-a3a9-fa3f30f4e8c1

/* (macro) define our custom UUID base (reverse byte-order) */
// 0xC1, 0xE8, 0xF4, 0x30, 0x3F, 0xFA, 0xA9, 0xA3, 0x69, 0x40, 0xFF, 0x55, 0x62, 0xB8, 0xCA, 0xF8
#define CUSTOM_SERVICE_UUID_BASE { 0xC1, 0xE8, 0xF4, 0x30, 0x3F, 0xFA, 0xA9, 0xA3, \
                                   0x69, 0x40, 0xFF, 0x55, 0x62, 0xB8, 0xCA, 0xF8 }

/* (macro) define 16-bit UUIDs for our custom service / characteristic (any value) */
// note: these values will be inserted later into the base UUID
#define CUSTOM_SERVICE_UUID     0x1900  // service
#define CUSTOM_VALUE_CHAR_UUID  0x1901  // value characteristic

/* (macro) define custom service instance */
#define BLE_CUS_DEF(_name) 
    static ble_cus_t _name;

/* (struct) define custom service init structure to hold all options/data for initialization */
typedef struct {
    uint8_t                         initial_custom_value;       // initial custom value
    ble_srv_cccd_security_mode_t    custom_value_char_attr_md;  // initial security level, characteristics
} ble_cus_init_t;

/* (struct) define custom service structure to hold status information of the service */
struct ble_cus_s {
    uint16_t                    service_handle;         // handle (alias) for custom service
    ble_gatts_char_handles_t    custom_value_handles;   // handles related to custom value characteristic
    uint16_t                    conn_handle;            // handle of current connection
    uint8_t                     uuid_type;
}

/* create a forward declaration of the custom service instance type (since this is a header) */
typedef struct ble_cus_s ble_cus_t;

/* declare custom service init function. */
/**@brief Function for initializing the Custom Service.
 *
 * @param[out]  p_cus       Custom Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_cus_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_cus_init(ble_cus_t* p_cus, const ble_cus_init_t* p_cus_init);