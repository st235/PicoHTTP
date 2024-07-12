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
    _tcp_server(std::make_unique<__internal::TcpServer>(max_connections)),
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
        __internal::Http11Parser http_parser;
        const auto& http_request = http_parser.fromRequest(payload);

        const auto* callback = this->findRouteCallback(http_request.getMethod(), http_request.getPath());

        HttpResponse response(connection_id, tcp_server, http_parser);

        if (!callback) {
            response.send("");
        } else {
            (*callback)(http_request, response);
        }

        return true;
    });


    _tcp_server->listen(_port);
}

}
