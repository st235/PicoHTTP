#include "http_server.h"

#include <stdio.h>
#include <cstdint>
#include <memory>
#include <string>
#include <cstring>

#include "tcp_server.h"
#include "http_status_code.h"
#include "http_headers.h"
#include "http1_1_parser.h"

namespace http {

void RunTest() {
    auto tcp_server = std::make_unique<__http_internal::TcpServer>(2);

    tcp_server->setOnConnectedCallback([](uint32_t connection_id) {
        printf("[TCP Server Callback] Connected: %d\n", connection_id);
    });

    tcp_server->setOnDataReceivedCallback([&tcp_server](uint32_t connection_id, uint8_t* data, uint16_t length) {
        std::string payload(data, data + length);
        delete[] data;

        printf("[TCP Server Callback] Raw request: %s\n", payload.c_str());

        __http_internal::Http11Parser http_parser;
        const auto& http_request = http_parser.fromRequest(payload);

        printf("[TCP Server Callback] Http request: %s\n", http_request.toString().c_str());

        std::string body = "Hello world!";

        HttpHeaders headers;
        headers["Content-Type"] = std::string("text/html; charset=utf-8");

        HttpResponse response(HttpStatusCode::OK, headers, body);

        std::string raw_response = http_parser.toResponse(response);
        printf("[TCP Server Callback] Raw response: %s\n", raw_response.c_str());

        const char* cresponse = raw_response.c_str();
        tcp_server->write(connection_id, cresponse, strlen(cresponse));
        tcp_server->close(connection_id);

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
