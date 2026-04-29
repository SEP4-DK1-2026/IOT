#include "network.h"
#include "wifi.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h> 
#include <time.h>

static char tcp_rx_buffer[700];
static volatile bool tcp_received = false;

// ================= CALLBACK =================
static void tcp_callback(void)
{
    //printf("\n--- SERVER RESPONSE ---\n%s\n------------------------\n", tcp_rx_buffer);
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

    if (wifi_command_create_TCP_connection("webhook.site", 80, tcp_callback, tcp_rx_buffer) != WIFI_OK)
    {
        printf("[NETWORK/TCP] ERROR - Connection setup failed \n");
        return;
    }
    printf("[NETWORK/TCP] Connected - Ready to send data\n");

    _delay_ms(500);

    // ================= JSON =================
    char json[128];
    char rain_num[10];
    char wind_speed_num[10];
    time_t seconds;
    
dtostrf(data->rain, 6, 2,rain_num);
dtostrf(data->wind_speed,6,2,wind_speed_num);
    sprintf(json,
            "{\"temp\":%d.%d,\"hum\":%d.%d,\"light\":%d, \"rainfall\":%s, \"windspeed\":%s, \"winddir\":%d \"time\" :%ld}",
            data->temp_i, data->temp_d,
            data->hum_i, data->hum_d,
            data->light,
            rain_num,
            wind_speed_num,
            data->wind_dir,
       time(&seconds));

    // ================= HTTP REQUEST =================
    char request[300];
    sprintf(request,
            "POST /09064a39-2237-40ed-8c7c-b888b02db8a4 HTTP/1.1\r\n"
            "Host: webhook.site\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s",
            strlen(json),
            json);

    // ================= SEND =================
    if (wifi_command_TCP_transmit((uint8_t *)request, strlen(request)) != WIFI_OK)
    {
        printf("[NETWORK/HTTP] ERROR - POST transmission failed (buffer overflow?)\n");
        wifi_command_close_TCP_connection();
        return;
    }

    printf("[NETWORK/HTTP] SUCCESS - POST sent (waiting for response)\n");

    // ================= WAIT FOR RESPONSE =================
    int timeout = 0;

    while (!tcp_received && timeout < 50)
    {
        _delay_ms(100);
        timeout++;
    }

    if (!tcp_received)
    {
        printf("[NETWORK/HTTP] WARNING - Server response timeout (waited 5s, got no reply)\n");
    }
    else
{
    printf("[NETWORK/HTTP] SUCCESS - Response received\n");
}

    // ================= CLOSE =================
    wifi_command_close_TCP_connection();

    printf("[NETWORK/HTTP] SUCCESS - Data transmitted and connection closed\n");
}