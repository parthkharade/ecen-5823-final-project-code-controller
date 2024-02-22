/*
 * ble.c
 *
 *  Created on: Feb 20, 2024
 *      Author: Parth
 */
#include "ble.h"

#include "gatt_db.h"
ble_data_struct_t ble_data;

#define ADV_INT_MIN_MS  250U
#define ADV_INT_MAX_MS  250U
#define ADV_VAL_SCALER  0.625F
#define ADV_INT_MIN_VAL ((ADV_INT_MAX_MS)/(ADV_VAL_SCALER))
#define ADV_INT_MAX_VAL ((ADV_INT_MAX_MS)/(ADV_VAL_SCALER))

#define CONN_INT_MIN_MS     75
#define CONN_INT_MAX_MS     75
#define CONN_SLV_LAT        4
#define CONN_SUPV_TIMOUT_MS (1 + (CONN_INT_MAX_MS)*(CONN_SLV_LAT))*((CONN_INT_MAX_MS)*2)

#define CONN_INT_SCALER     1.25F
#define CONN_SUPV_TIMOUT_SCALER 10

#define CONN_INT_MIN_VAL  ((CONN_INT_MIN_MS)/(CONN_INT_SCALER))
#define CONN_INT_MAX_VAL  ((CONN_INT_MAX_MS)/(CONN_INT_SCALER))
#define CONN_SUPV_TIMOUT_VAL  ((CONN_SUPV_TIMOUT_MS)/(CONN_SUPV_TIMOUT_SCALER))

ble_data_struct_t *get_ble_data(){
  return &ble_data;
}

void handle_ble_event(sl_bt_msg_t *evt){
  switch(SL_BT_MSG_ID(evt->header)){
    case sl_bt_evt_system_boot_id:
      sl_bt_system_get_identity_address(&(ble_data.myAddress), 0);
      sl_bt_advertiser_create_set(&ble_data.advertisingSetHandle);
      sl_bt_advertiser_set_timing(ble_data.advertisingSetHandle, ADV_INT_MIN_VAL, ADV_INT_MAX_VAL, 0, 0);
      sl_bt_advertiser_start(ble_data.advertisingSetHandle, sl_bt_advertiser_general_discoverable, sl_bt_advertiser_connectable_scannable);
      break;
    case sl_bt_evt_connection_opened_id:
      sl_bt_advertiser_stop(ble_data.advertisingSetHandle);
      ble_data.connectionHandle = evt->data.evt_connection_opened.connection;
      ble_data.connection_open = true;
      sl_bt_connection_set_parameters(ble_data.connectionHandle, CONN_INT_MIN_VAL, CONN_INT_MAX_VAL, CONN_SLV_LAT, CONN_SUPV_TIMOUT_VAL, 0, 0);
      break;
    case sl_bt_evt_connection_closed_id:
      ble_data.connection_open = false;
      sl_bt_advertiser_start(ble_data.advertisingSetHandle, sl_bt_advertiser_general_discoverable, sl_bt_advertiser_connectable_scannable);
      break;
    case sl_bt_evt_connection_parameters_id:
      // LOG STUFF HERE
      break;
    case sl_bt_evt_system_external_signal_id:
      //IGNORE THIS OVER HERE This is for our temperature state machine.
      break;
      // Events for only servers
    case sl_bt_evt_gatt_server_characteristic_status_id:
      if(evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement){
          if(evt->data.evt_gatt_server_characteristic_status.client_config_flags
              == sl_bt_gatt_indication){
              ble_data.ok_to_send_htm_indications = true;
          }
          else{
              ble_data.ok_to_send_htm_indications = false;
          }
      }

      ble_data.indication_in_flight = false;
      break;
    case sl_bt_evt_gatt_server_indication_timeout_id:
      ble_data.indication_in_flight = false;
      break;
  }
}
