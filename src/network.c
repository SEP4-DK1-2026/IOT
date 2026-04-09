#include "network.h"
#include "wifi.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

static char tcp_rx_buffer[256];
static volatile bool tcp_received = false;

// ================= CALLBACK =================
static void tcp_callback(void)
{
    printf("\n--- SERVER RESPONSE ---\n%s\n------------------------\n", tcp_rx_buffer);
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
        printf("WiFi failed\n");
        return;
    }

    printf("WiFi connected!\n");

    wifi_command_set_to_single_Connection();
}

// ================= SEND DATA =================
void send_sensor_data(sensor_data_t *data)
{
    tcp_received = false;
    memset(tcp_rx_buffer, 0, sizeof(tcp_rx_buffer));

   
    if (wifi_command_create_TCP_connection("webhook.site", 80, tcp_callback, tcp_rx_buffer) != WIFI_OK)
    {
        printf("TCP connection failed\n");
        return;
    }

    printf("TCP connected\n");

    _delay_ms(500);

    // ================= JSON =================
    char json[128];
    sprintf(json,
            "{\"temp\":%d.%d,\"hum\":%d.%d,\"light\":%d}",
            data->temp_i, data->temp_d,
            data->hum_i, data->hum_d,
            data->light);

    // ================= HTTP REQUEST =================
    char request[300];
    sprintf(request,
            "POST /08ebc290-d248-4537-90e4-de922467ec15 HTTP/1.1\r\n"
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
        printf("POST failed\n");
        wifi_command_close_TCP_connection();
        return;
    }

    printf("POST sent!\n");

    // ================= WAIT FOR RESPONSE =================
    int timeout = 0;

    while (!tcp_received && timeout < 50)
    {
        _delay_ms(100);
        timeout++;
    }

    if (!tcp_received)
    {
        printf("No response from server\n");
    }

    // ================= CLOSE =================
    wifi_command_close_TCP_connection();

    printf("Connection closed\n\n");
}