/***********************************************
 * network.h
 *  Network communication interface
 * 
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: SPE4_API
 **********************************************/

#pragma once
#include "sensor_manager.h"

/* Initialize WiFi and network module */
void network_init(void);

/* Send sensor data to server */
void send_sensor_data(sensor_data_t* data);