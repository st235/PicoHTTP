#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "http_server.h"

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }

    printf("WIFI_SSID %s WIFI_PASSWORD %s\n", WIFI_SSID, WIFI_PASSWORD);

    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");
    }

    http::HttpServer server(3036);

    server.onGet("/", [](const auto& request, auto& response) {
        response.addHeader("Content-Type", "text/html; charset=utf-8");
        response.send("Hello World");
    });

    server.onGet("/ping", [](const auto& request, auto& response) {
        response.addHeader("Content-Type", "text/html; charset=utf-8");
        response.send("pong");
    });

    server.onPost("/ping", [](const auto& request, auto& response) {
        response.addHeader("Content-Type", "text/html; charset=utf-8");
        response.send("post pong");
    });

    server.onGet("/echo", [](const auto& request, auto& response) {
        response.addHeader("Content-Type", "text/html; charset=utf-8");

        if (request.hasQuery("q")) {
            response.send(request.getQuery("q"));
        } else {
            response.send("q is empty");
        }
    });

    server.start();
    
    while (true) {
        printf("Sleeping...\n");
        sleep_ms(1000);
    }
}