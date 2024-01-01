#include "http_server.h"

#include <stdio.h>
#include <cstdint>
#include <memory>
#include <string>

#include "tcp_server.h"
#include "http1_1_parser.h"

namespace http {

void RunTest() {
    auto tcp_server = std::make_unique<__http_internal::TcpServer>(2);

    tcp_server->setOnConnectedCallback([](uint32_t connection_id) {
        printf("[TCP Server Callback] Connected: %d\n", connection_id);
    });

    tcp_server->setOnDataReceivedCallback([](uint32_t connection_id, uint8_t* data, uint16_t length) {
        std::string payload(data, data + length);
        delete[] data;

        __http_internal::Http11Parser http_parser;
        const auto& http_request = http_parser.fromRequest(payload);

        printf("[TCP Server Callback] Http request: %s\n", http_request.toString().c_str());
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
