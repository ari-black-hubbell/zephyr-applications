#ifndef SERVICES_H_
#define SERVICES_H_

/* services.h */

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

/* the length of transmission buffers */
#define MAX_TRANSMIT_SIZE 240

/* 
 * UUIDs for Standard BLE Services 
 */

/* UUID base: 00000000-0000-1000-8000-00805F9B34FB */
// /* Generic Access: 1800 */
// #define BT_UUID_GAP_VAL     0x1800
// #define BT_UUID_GAP         BT_UUID_DECLARE_16(BT_UUID_GAP_VAL)

// /* Generic Attribute: 1801 */
// #define BT_UUID_GATT_VAL    0x1801
// #define BT_UUID_GATT        BT_UUID_DECLARE_16(BT_UUID_GATT_VAL)

// /* Device Information: 180A */
// #define BT_UUID_DIS_VAL     0x180a
// #define BT_UUID_DIS         BT_UUID_DECLARE_16(BT_UUID_DIS_VAL)

// /* Battery Service: 180F */
// #define BT_UUID_BAS_VAL     0x180f
// #define BT_UUID_BAS         BT_UUID_DECLARE_16(BT_UUID_BAS_VAL)

/* 
 * UUIDs for Standard BLE Characteristics 
 */

/* Device Name (GAP): 2a00 */
// #define BT_UUID_GAP_DN_VAL          0x2a00
// #define BT_UUID_GAP_DN              BT_UUID_DECLARE_16(BT_UUID_GAP_DN_VAL)

// /* Appearance (GAP): 2a01 */
// #define BT_UUID_GAP_APP_VAL         0x2a01
// #define BT_UUID_GAP_APP             BT_UUID_DECLARE_16(BT_UUID_GAP_APP_VAL)

// /* Service Change Request (GATT): 2a05 */
// #define BT_UUID_GATT_SCR_VAL        0x2a05
// #define BT_UUID_GATT_SCR            BT_UUID_DECLARE_16(BT_UUID_GATT_SCR_VAL)

// /* Client Supported Features (GATT): 2b29 */
// #define BT_UUID_GATT_CSF_VAL        0x2b29
// #define BT_UUID_GATT_CSF            BT_UUID_DECLARE_16(BT_UUID_GATT_CSF_VAL)

// /* Database Hash (GATT): 2b2a */
// #define BT_UUID_GATT_DBH_VAL        0x2b2a
// #define BT_UUID_GATT_DBH            BT_UUID_DECLARE_16(BT_UUID_GATT_DBH_VAL)

// /* System ID (DIS): 2129 */
// #define BT_UUID_DIS_SID_VAL         0x2129
// #define BT_UUID_DIS_SID             BT_UUID_DECLARE_16(BT_UUID_DIS_SID_VAL)

// /* Firmware Revision (DIS): 2a26 */
// #define BT_UUID_DIS_FR_VAL          0x2a26
// #define BT_UUID_DIS_FR              BT_UUID_DECLARE_16(BT_UUID_DIS_FR_VAL)

// /* Manufacturer's Name String (DIS): 2a29 */
// #define BT_UUID_DIS_MNS_VAL         0x2a29
// #define BT_UUID_DIS_MNS             BT_UUID_DECLARE_16(BT_UUID_DIS_MNS_VAL)

// /* Battery Level (BAS): 2a19 */
// #define BT_UUID_BAS_BL_VAL          0x2a19
// #define BT_UUID_BAS_BL              BT_UUID_DECLARE_16(BT_UUID_BAS_BL_VAL)

/* 
 * UUIDs for Custom BLE Services 
 */

/* Device Settings: 25af4310-87b5-432f-bbde-6ac650235235 */
#define BT_UUID_DS      BT_UUID_128_ENCODE(0x25af4310, 0x87b5, 0x432f, 0xbbde, 0x6ac650235235)

/* Sensor Information: c60ca733-bd6c-787d-99f4-70460f1cb78c */
#define BT_UUID_SI      BT_UUID_128_ENCODE(0xc60ca733, 0xbd6c, 0x787d, 0x99f4, 0x70460f1cb78c)

/* Wireless Security: 92467915-954e-7f30-f090-95a3f05a54b1 */
#define BT_UUID_WLS     BT_UUID_128_ENCODE(0x92467915, 0x954e, 0x7f30, 0xf090, 0x95a3f05a54b1)

/* OTA: 1d14d6ee-fd63-4fa1-bfa4-8f47b42119f0 */
#define BT_UUID_OTA     BT_UUID_128_ENCODE(0x1d14d6ee, 0xfd63, 0x4fa1, 0xbfa4, 0x8f47b42119f0)

/* 
 * UUIDs for Custom BLE Characteristics 
 */

/* LED (DS): 0db91d25-3fb1-4be0-98fe-f9db8fcc6bb6 */
#define BT_UUID_DS_LED              BT_UUID_128_ENCODE(0x0db91d25, 0x3fb1, 0x4be0, 0x98fe, 0xf9db8fcc6bb6)

#define BT_UUID_DS_DES              BT_UUID_128_ENCODE(0x0db91d25, 0x3fb1, 0x4be0, 0x98fe, 0xf9db8fcc6bb7)

/* Fault Current: (DS): 26d0392e-52f9-1745-f53a-0603066a3f73 */
#define BT_UUID_DS_FC               BT_UUID_128_ENCODE(0x26d0392e, 0x52f9, 0x1745, 0xf53a, 0x0603066a3f73)

/* Low Battery Count (DS): 128e799a-b023-634b-d08a-e70c38437c65 */
#define BT_UUID_DS_LBC              BT_UUID_128_ENCODE(0x128e799a, 0xb023, 0x634b, 0xd08a, 0xe70c38437c65)

/* Alarm Timeout (DS): b9919d3f-9b87-1819-5218-6757f9e87a75 */
#define BT_UUID_DS_ALT              BT_UUID_128_ENCODE(0xb9919d3f, 0x9b87, 0x1819, 0x5218, 0x6757f9e87a75)

/* Alarm Clear (DS): 5c5c1dd6-df18-2c25-5fd0-a22989f57f2a */
#define BT_UUID_DS_ALC              BT_UUID_128_ENCODE(0x5c5c1dd6, 0xdf18, 0x2c25, 0x5fd0, 0xa22989f57f2a)

/* Sensor Health Data (SI): 84755f2f-8b18-3691-1f5c-f3a0859df437 */
#define BT_UUID_SI_SHD              BT_UUID_128_ENCODE(0x84755f2f, 0x8b18, 0x3691, 0x1f5c, 0xf3a0859df437)

/* IV (WLS): 90ba68c6-9d23-8999-babd-c004a49281d2 */
#define BT_UUID_WLS_IV              BT_UUID_128_ENCODE(0x90ba68c6, 0x9d23, 0x8999, 0xbabd, 0xc004a49281d2)

/* Passkey (WLS): 1a337328-6bfa-60ee-761a-2021d5e8cfc5 */
#define BT_UUID_WLS_PK              BT_UUID_128_ENCODE(0x1a337328, 0x6bfa, 0x60ee, 0x761a, 0x2021d5e8cfc5)

/* OTA Control (OTAC): f7bf3564-fb6d-4e53-88a4-5e37e0326063 */
#define BT_UUID_OTA_OTAC            BT_UUID_128_ENCODE(0xf7bf3564, 0xfb6d, 0x4e53, 0x88a4, 0x5e37e0326063)

#endif /* services.h */