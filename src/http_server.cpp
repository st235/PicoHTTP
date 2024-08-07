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

Server::Server(uint16_t port, uint8_t max_connections):
    _port(port),
    _max_connections(max_connections),
    _tcp_server(max_connections),
    _routes() {

    _tcp_server.setOnConnectedCallback([](uint32_t connection_id) {
        PLOGD("Connected: %d\n", connection_id);
    });

    _tcp_server.setOnClosedCallback([](uint32_t connection_id) {
        PLOGD("Closed: %d\n", connection_id);
    });
}

const Server::OnRouteCallback* Server::findRouteCallback(const Method& method,
                                                         const std::string& route) const {
    if (_routes.find(method) == _routes.end()) {
        return nullptr;
    }

    if (_routes.at(method).find(route) == _routes.at(method).end()) {
        return nullptr;
    }

    return &(_routes.at(method).at(route));
}

void Server::onMethod(const Method& method,
                          const std::string& route,
                          OnRouteCallback callback) {
    if (_routes.find(method) == _routes.end()) {
        _routes[method] = std::unordered_map<std::string, OnRouteCallback>();
    }

    _routes[method][route] = callback;
}

void Server::start() {
    _tcp_server.setOnDataReceivedCallback([=](uint32_t connection_id, uint8_t* data, uint16_t length) {
        std::string payload(data, data + length);

        // TODO(st235): join different parsers together.
        __internal::Http11Parser http_parser;
        const auto& http_request = http_parser.fromRequest(payload);

        const auto* callback = this->findRouteCallback(http_request.getMethod(), http_request.getPath());

        Response response(connection_id, *this);

        if (!callback) {
            response.send("");
        } else {
            (*callback)(http_request, response);
        }

        return true;
    });


    _tcp_server.listen(_port);
}

void Server::send(const Response& response,
                  const std::string& body) const {
    // TODO(st235): add parser interface.
    __internal::Http11Parser http_parser;

    auto connection_id = response.getConnectionId();
    std::string parsed_response = http_parser.toResponse(response, body);
    const char* raw_response = parsed_response.c_str();

    _tcp_server.write(connection_id, raw_response, strlen(raw_response));
    _tcp_server.close(connection_id);
}

}
