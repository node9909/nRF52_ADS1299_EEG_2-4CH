
/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup ble_sdk_srv_eeg Biopotential Measurement Service
 * @{
 * @ingroup ble_sdk_srv
 * @brief Biopotential Measurement Service module.
 *
 * @details This module implements the Biopotential Measurement Service with the Voltage characteristic.
 *          During initialization it adds the Biopotential Measurement Service and Voltage characteristic
 *          to the BLE stack dataBISe. Optionally it can also add a Report Reference descriptor
 *          to the Battery Level characteristic (used when including the Biopotential Measurement Service in
 *          the HID service).
 *
 *          If specified, the module will support notification of the Battery Level characteristic
 *          through the ble_eeg_battery_level_update() function.
 *          If an event handler is supplied by the application, the Biopotential Measurement Service will
 *          generate Biopotential Measurement Service events to the application.
 *
 * @note The application must propagate BLE stack events to the Biopotential Measurement Service module by calling
 *       ble_eeg_on_ble_evt() from the from the @ref ble_stack_handler callback.
 */

#ifndef BLE_EEG_H__
#define BLE_EEG_H__

#include "ble.h"
#include "ble_srv_common.h"
#include "sdk_config.h"
#include <stdint.h>

// Base UUID
#define BMS_UUID_BASE \
  { 0x57, 0x80, 0xD2, 0x94, 0xA3, 0xB2, 0xFE, 0x39, 0x5F, 0x87, 0xFD, 0x35, 0x00, 0x00, 0x8B, 0x22 }

// Service UUID
#define BLE_UUID_BIOPOTENTIAL_EEG_MEASUREMENT_SERVICE 0xEEF0

// Characteristic UUIDs
#define BLE_UUID_EEG_CONFIG 0xEEFF
#define BLE_UUID_EEG_CH1_CHAR 0xEEF1
#define BLE_UUID_EEG_CH2_CHAR 0xEEF2
#define BLE_UUID_EEG_CH3_CHAR 0xEEF3
#define BLE_UUID_EEG_CH4_CHAR 0xEEF4

#if SAMPLE_RATE == 250
#define EEG_PACKET_LENGTH 60
#else
#define EEG_PACKET_LENGTH 246
#endif

//Forward declaration of ble_eeg_t type:
typedef struct ble_eeg_s ble_eeg_t;

// Setup handler for writes:
typedef void (*ble_eeg_write_config_handler_t)(uint16_t conn_handle, ble_eeg_t *p_eeg, uint8_t *data);

//Setup handler:
typedef struct {
  ble_eeg_write_config_handler_t eeg_config_handler;
} ble_eeg_init_t;

/**@brief Biopotential Measurement Service init structure. This contains all options and data needed for
 *        initialization of the service. */
struct ble_eeg_s {
  uint8_t uuid_type;
  uint16_t conn_handle;                                 /**< Event handler to be called for handling events in the Biopotential Measurement Service. */
  uint16_t service_handle;                              /**< Handle of ble Service (as provided by the BLE stack). */
  ble_gatts_char_handles_t ads1299_config_char_handles; /**< Handles related to the our body V measure characteristic. */
  ble_eeg_write_config_handler_t eeg_config_handler;
  uint8_t ads1299_current_configuration[23];
  ble_gatts_char_handles_t eeg_ch1_handles;             /**< Handles related to the our body V measure characteristic. */
  ble_gatts_char_handles_t eeg_ch2_handles;             /**< Handles related to the our body V measure characteristic. */
  ble_gatts_char_handles_t eeg_ch3_handles;             /**< Handles related to the our body V measure characteristic. */
  ble_gatts_char_handles_t eeg_ch4_handles;             /**< Handles related to the our body V measure characteristic. */
  uint8_t eeg_ch1_buffer[EEG_PACKET_LENGTH];            //246 or 4* =
  uint8_t eeg_ch2_buffer[EEG_PACKET_LENGTH];            //246 or 4* =
  uint8_t eeg_ch3_buffer[EEG_PACKET_LENGTH];
  uint8_t eeg_ch4_buffer[EEG_PACKET_LENGTH];
  uint16_t eeg_ch1_count;
};

void ble_eeg_service_init(ble_eeg_t *p_eeg, const ble_eeg_init_t *init);

/**@brief Biopotential Measurement Service BLE stack event handler.
 *
 * @details Handles all events from the BLE stack of interest to the Biopotential Measurement Service.
 *
 * @param[in]   p_eeg      Biopotential Measurement Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_eeg_on_ble_evt(ble_eeg_t *p_eeg, ble_evt_t *p_ble_evt);

/**@brief function for updating/notifying BLE of new value.
*
*/
void ble_eeg_update_configuration(ble_eeg_t *p_eeg, bool notify);
void ble_eeg_update_1ch_v2(ble_eeg_t *p_eeg);
void ble_eeg_update_4ch(ble_eeg_t *p_eeg);

#endif // BLE_EEG_H__