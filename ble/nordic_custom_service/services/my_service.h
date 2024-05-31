
/* my_service.h */

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

/* declaration: initialization function to call from main. */
int my_service_init(void);

/* randomized service UUID: 5efcd59b-9108-4b83-b7b5-a495 3cd9a0e1. 
 * note: UUIDs need to have LSB ordering.
*/
#define MY_SERVICE_UUID 0xe1, 0xa0, 0xd9, 0x3c, 0x95, 0xa4, 0xb5, \
            0xb7, 0x83, 0x4b, 0x08, 0x91, 0x9b, 0xd5, 0xfc, 0x5e

/* TX characteristic UUID: 6fb2e701-22a6-4c90-ba6c-92ca 22043938. */
#define TX_CHARACTERISTIC_UUID 0x38, 0x39, 0x04, 0x22, 0xca, 0x92, 0x6c, \
                    0xba, 0x90, 0x4c, 0xa6, 0x22, 0x01, 0xe7, 0xb2, 0x6f

/* RX characteristic UUID: d3d5557e-9bdb-4c03-8b3e-6fc0 663bf291. */
#define RX_CHARACTERISTIC_UUID 0x91, 0xf2, 0x3b, 0x66, 0xc0, 0x6f, 0x3e, \
                    0x8b, 0x03, 0x4c, 0xdb, 0x9b, 0x7e, 0x55, 0xd5, 0xd3