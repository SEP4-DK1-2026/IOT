#include "network.h"
#include "wifi.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>

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

    int attempts = 0;

    while (wifi_command_join_AP("Namnam", "Benjamin") != WIFI_OK)
    {
        _delay_ms(2000);
        attempts++;

        if (attempts >= 10)
        {
            break;
        }
    }

    if (attempts >= 10)
    {
        return; // VIGTIG: Giv op her!
    }

    wifi_command_set_to_single_Connection();
}

// ================= SEND DATA =================
void send_sensor_data(sensor_data_t *data)
{
    tcp_received = false;
    memset(tcp_rx_buffer, 0, sizeof(tcp_rx_buffer));

    int attempts = 0;
    while (wifi_command_create_TCP_connection("webhook.site", 80, tcp_callback, tcp_rx_buffer) != WIFI_OK)
    {
        _delay_ms(500);
        attempts++;

        if (attempts >= 10)
        {
            break;
        }
    }

    if (attempts >= 10)
    {

        return;
    }

    _delay_ms(500);

    // ================= JSON =================
    char json[128];
    char rain_num[10];
    char wind_speed_num[10];

    dtostrf(data->rain, 6, 2, rain_num);
    dtostrf(data->wind_speed, 6, 2, wind_speed_num);
    sprintf(json,
            "{\"temp\":%d.%d,\"hum\":%d.%d,\"light\":%d, \"rainfall\":%s, \"windspeed\":%s, \"winddir\":%d}",
            data->temp_i, data->temp_d,
            data->hum_i, data->hum_d,
            data->light,
            rain_num,
            wind_speed_num,
            data->wind_dir);

    // ================= HTTP REQUEST =================
    char request[300];
    sprintf(request,
            "POST /d0e03901-71fa-4657-80ed-51deaa028eb3 HTTP/1.1\r\n"
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
        wifi_command_close_TCP_connection();
        return;
    }

    // ================= WAIT FOR RESPONSE =================
    int timeout = 0;

    while (!tcp_received && timeout < 20) // Vent 5 sekunder (20 * 250ms)
    {
        _delay_ms(100);
        timeout++;
    }

    // ================= CLOSE =================
    wifi_command_close_TCP_connection();
}