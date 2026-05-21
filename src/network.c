/***********************************************
 * network.c
 *  Network communication implementation
 * 
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: SPE4_API
 **********************************************/

 #include "network.h"
#include "wifi.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>

static char tcp_rx_buffer[700];

/* Set to true when TCP response is received */
static volatile bool tcp_received = false;

// ================= CALLBACK =================

/* Called when TCP data is received */
static void tcp_callback(void)
{
    tcp_received = true;
}

// ================= INIT =================
void network_init(void)
{
     /* Initialize WiFi module */
    wifi_init();

    _delay_ms(4000);

     /* Disable command echo */
    wifi_command_disable_echo();

     /* Set WiFi mode to station */
    wifi_command_set_mode_to_1();

    int attempts = 0;

    /* Try connecting to WiFi */
    while (wifi_command_join_AP("TorstedVVS", "Virada1373") != WIFI_OK)
    {
        _delay_ms(2000);
        attempts++;

                /* Stop after 10 attempts */
        if (attempts >= 10)
        {
            break;
        }
    }

    /* Give up if connection failed */
    if (attempts >= 10)
    {
        return;  
    }
/* Use single TCP connection mode */
    wifi_command_set_to_single_Connection();
}

// ================= SEND DATA =================
void send_sensor_data(sensor_data_t *data)
{
     /* Reset receive state */
    tcp_received = false;

    memset(tcp_rx_buffer, 0, sizeof(tcp_rx_buffer));

    int attempts = 0;

     /* Create TCP connection */
    while (wifi_command_create_TCP_connection("20.208.6.0", 80, tcp_callback, tcp_rx_buffer) != WIFI_OK)
    {
        printf("[NETWORK/TCP] ERROR - Connection setup failed \n");
        _delay_ms(500);
        attempts++;

           /* Stop after 10 attempts */
        if (attempts >= 10)
        {
            printf("[NETWORK/TCP] ERROR - Failed to establish TCP connection after 10 attempts, giving up gg\n");
            break;
        }
    }

        /* Stop if connection failed */

    if (attempts >= 10)
    {
        printf("[NETWORK/WiFi] FATAL - Could not connect\n");
        return;
    }

    printf("[NETWORK/TCP] Connected - Ready to send data\n");

    _delay_ms(500);

    // ================= JSON =================

     char json[512];
    char rain_num[10];
    char wind_speed_num[10];
    char light_num[15];

      /* Convert float values to strings */
    dtostrf(data->rain, 1, 2, rain_num);
    dtostrf(data->wind_speed, 1, 2, wind_speed_num);
    dtostrf(data->light, 1, 2, light_num);


    /* Create JSON payload */
    sprintf(json,
            "{\"temp\":%d.%d,\"hum\":%d.%d,\"light\":%s, \"rain\":%s, \"wspeed\":%s, \"wdir\":%d}",
            data->temp_i, data->temp_d,
            data->hum_i, data->hum_d,
            light_num,
            rain_num,
            wind_speed_num,
            data->wind_dir);

    // ================= HTTP REQUEST =================
    char request[512];

    /* Build HTTP POST request */
    sprintf(request,
    "POST /sensor HTTP/1.1\r\n"
    "Host: iot-weather-api-hyd8ekgcb4hkb2as.switzerlandnorth-01.azurewebsites.net\r\n"
    "Content-Type: application/json\r\n"
    "Content-Length: %u\r\n"
    "Connection: close\r\n"
    "\r\n"
    "%s",
    (unsigned int)strlen(json),
    json);


    // ================= SEND =================

      /* Send HTTP request */
    if (wifi_command_TCP_transmit((uint8_t *)request, strlen(request)) != WIFI_OK)
    {
        printf("[NETWORK/HTTP] ERROR - POST transmission failed (buffer overflow?)\n");
        wifi_command_close_TCP_connection();
        return;
    }

    printf("[NETWORK/HTTP] SUCCESS - POST sent (waiting for response) %d %d\n", strlen(request), strlen(json));

    // ================= WAIT FOR RESPONSE =================
    int timeout = 0;

     /* Wait for server response */
    while (!tcp_received && timeout < 20) // Vent 5 sekunder (20 * 250ms)
    {
        _delay_ms(100);
        timeout++;
    }

      /* Check if response was received */
    if (!tcp_received)
    {
        printf("[NETWORK/HTTP] WARNING - Server response timeout (waited 2s, got no reply)\n");
    }
    else
    {
        printf("[NETWORK/HTTP] SUCCESS - Response received\n");
    }

    // ================= CLOSE =================
     /* Close TCP connection */
    wifi_command_close_TCP_connection();

    printf("[NETWORK/HTTP] SUCCESS - Data transmitted and connection closed\n");
}