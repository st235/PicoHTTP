#include "http_server.h"

#include <stdio.h>
#include <cstdint>
#include <memory>
#include <string>
#include <cstring>

#include "log.h"
#include "http_status_code.h"
#include "http_headers.h"
#include "http1_1_parser.h"

namespace http {

HttpServer::HttpServer(uint16_t port, uint8_t max_connections):
    _port(port),
    _max_connections(max_connections),
    _tcp_server(std::make_unique<__http_internal::TcpServer>(max_connections)),
    _routes() {

    _tcp_server->setOnConnectedCallback([](uint32_t connection_id) {
        PLOGD("Connected: %d\n", connection_id);
    });

    _tcp_server->setOnClosedCallback([](uint32_t connection_id) {
        PLOGD("Closed: %d\n", connection_id);
    });
}

const HttpServer::OnRouteCallback* HttpServer::findRouteCallback(const HttpMethod& method,
                                               const std::string& route) const {
    if (_routes.find(method) == _routes.end()) {
        return nullptr;
    }

    if (_routes.at(method).find(route) == _routes.at(method).end()) {
        return nullptr;
    }

    return &(_routes.at(method).at(route));
}

void HttpServer::onMethod(const HttpMethod& method,
                          const std::string& route,
                          OnRouteCallback callback) {
    if (_routes.find(method) == _routes.end()) {
        _routes[method] = std::unordered_map<std::string, OnRouteCallback>();
    }

    _routes[method][route] = callback;
}

void HttpServer::start() {
    auto* tcp_server = _tcp_server.get();

    _tcp_server->setOnDataReceivedCallback([=, tcp_server](uint32_t connection_id, uint8_t* data, uint16_t length) {
        std::string payload(data, data + length);

        // TODO(st235): add parser interface.
        __http_internal::Http11Parser http_parser;
        const auto& http_request = http_parser.fromRequest(payload);

        const auto* callback = this->findRouteCallback(http_request.getMethod(), http_request.getPath());

        std::string body = "Hello world!";

        HttpHeaders headers;
        headers["Content-Type"] = std::string("text/html; charset=utf-8");

        HttpResponse response(HttpStatusCode::OK, headers, body);

        std::string raw_response = http_parser.toResponse(response);

        (*callback)(http_request, response);

        const char* cresponse = raw_response.c_str();
        tcp_server->write(connection_id, cresponse, strlen(cresponse));
        tcp_server->close(connection_id);

        return true;
    });


    _tcp_server->listen(_port);
}

}
