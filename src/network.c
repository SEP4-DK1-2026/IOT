#include "network.h"
#include "wifi.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h> 

static char tcp_rx_buffer[700];
static volatile bool tcp_received = false;

// ================= CALLBACK =================
static void tcp_callback(void)
{
    tcp_received = true;
}

// ================= INIT =================
void network_init(void)
{
    wifi_init();
    _delay_ms(4000);

    wifi_command_disable_echo();
    wifi_command_set_mode_to_1();

    if (wifi_command_join_AP("Namnam", "Benjamin") != WIFI_OK)
    {
        printf("[NETWORK/WiFi] ERROR - Failed to join 'Namnam'\n");
        return;
    }

    printf("[NETWORK/WiFi] Connected successfully: Namnam\n");

    wifi_command_set_to_single_Connection();
}

// ================= SEND DATA =================
void send_sensor_data(sensor_data_t *data)
{
    tcp_received = false;
    memset(tcp_rx_buffer, 0, sizeof(tcp_rx_buffer));

    

     // ================= DNS TEST =================
    printf("[NETWORK] Testing DNS lookup...\n");

    WIFI_ERROR_MESSAGE_t dns_test =
        wifi_command_AT(
            "AT+CIPDOMAIN=\"iot-container.yellowglacier-07102b9a.switzerlandnorth.azurecontainerapps.io\"",
            10);

    if (dns_test != WIFI_OK)
    {
        printf("[NETWORK] ERROR - DNS lookup failed: %d\n", dns_test);
        return;
    }

    printf("[NETWORK] DNS lookup success\n");
    
    WIFI_ERROR_MESSAGE_t wifierror = wifi_command_create_TCP_connection("20.208.6.0", 80, tcp_callback, tcp_rx_buffer);
  if (wifierror != WIFI_OK)
    {
        printf("[NETWORK/TCP] ERROR - Connection setup failed: %d \n", wifierror);
        
        return;
    }
    printf("[NETWORK/TCP] Connected - Ready to send data\n");

    _delay_ms(500);

    // ================= JSON =================
    char json[512];
    char rain_num[10];
    char wind_speed_num[10];
    
dtostrf(data->rain, 1, 2,rain_num);
dtostrf(data->wind_speed,1,2,wind_speed_num);
    sprintf(json,
            "{\"temp\":%d.%d,\"hum\":%d.%d,\"light\":%d, \"rain\":%d, \"wspeed\":%d, \"wdir\":%d}",
            data->temp_i, data->temp_d,
            data->hum_i, data->hum_d,
            data->light,
            12,
            25,
            data->wind_dir);

    // ================= HTTP REQUEST =================
    char request[512];
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
printf("\n========== HTTP REQUEST ==========\n");
printf("%s\n", request);
printf("==================================\n");
printf("REQUEST LENGTH: %u\n", strlen(request));

    if (wifi_command_TCP_transmit((uint16_t *)request, strlen(request)) != WIFI_OK)
    {
        printf("[NETWORK/HTTP] ERROR - POST transmission failed (buffer overflow?)\n");
        wifi_command_close_TCP_connection();
        return;
    }

    printf("[NETWORK/HTTP] SUCCESS - POST sent (waiting for response)\n");

    // ================= WAIT FOR RESPONSE =================
    int timeout = 0;

    while (!tcp_received && timeout < 500) // Vent 5 sekunder (20 * 250ms)
    {
        _delay_ms(100);
        timeout++;
    }

    if (!tcp_received)
    {
        printf("[NETWORK/HTTP] WARNING - Server response timeout (waited 2s, got no reply)\n");
    }
    else
{
    printf("[NETWORK/HTTP] SUCCESS - Response received\n");
}

    // ================= CLOSE =================
    wifi_command_close_TCP_connection();

    printf("[NETWORK/HTTP] SUCCESS - Data transmitted and connection closed\n");
}