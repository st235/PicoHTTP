#include "http_server.h"

#include <stdio.h>
#include <cstdint>
#include <memory>
#include <string>

#include "tcp_server.h"

namespace http {

void RunTest() {
    auto tcp_server = std::make_unique<__http_internal::TcpServer>(1);

    tcp_server->setOnConnectedCallback([](uint32_t connection_id) {
        printf("[TCP Server Callback] Connected: %d\n", connection_id);
    });

    tcp_server->setOnDataReceivedCallback([](uint32_t connection_id, uint8_t* data, uint16_t length) {
        std::string payload(data, data + length);
        printf("[TCP Server Callback] Connection id (%d), received data (of length %d): %s\n", connection_id, length, payload.c_str());
        return true;
    });

    tcp_server->setOnClosedCallback([](uint32_t connection_id) {
        printf("[TCP Server Callback] Closed: %d\n", connection_id);
    });

    tcp_server->listen(/* port= */ 3036);

    // workaround to keep the server running.
    while(true) {}
}

}
