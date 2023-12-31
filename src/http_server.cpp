#include "http_server.h"

#include <stdio.h>
#include <cstdint>
#include <memory>

#include "tcp_server.h"

namespace http {

void RunTest() {
    auto tcp_server = std::make_unique<__http_internal::TcpServer>(1);

    tcp_server->setOnConnectedCallback([](uint32_t connection_id) {
        printf("Connected: %d\n", connection_id);
    });

    tcp_server->setOnDataReceivedCallback([](uint32_t connection_id, char* data, uint16_t length) {
        printf("Connection id (%d), received data: %s\n", connection_id, data);
        return true;
    });

    tcp_server->setOnClosedCallback([](uint32_t connection_id) {
        printf("Closed: %d\n", connection_id);
    });

    tcp_server->listen(/* port= */ 3036);
    while(true) {}
}

}
