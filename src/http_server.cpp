#include "http_server.h"

#include <cstdint>
#include <cstring>
#include <memory>
#include <stdio.h>
#include <string>

#include "http_headers.h"
#include "http_status_code.h"
#include "log.h"

namespace http {

Server::Server(uint16_t port, uint8_t max_connections):
    _port(port),
    _max_connections(max_connections),
    _coder(),
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

        // TODO(st235): handle no value parsed.
        const auto& request_opt = _coder.decode(payload);
        const auto& request = request_opt.value();

        const auto* callback = this->findRouteCallback(request.getMethod(), request.getPath());

        Response response(connection_id, request.getProtocolVersion(), *this);

        if (!callback) {
            response.send("");
        } else {
            (*callback)(request, response);
        }

        return true;
    });


    _tcp_server.listen(_port);
}

void Server::send(const Response& response,
                  const std::string& body,
                  const StatusCode& status_code) const {
    std::string parsed_response = _coder.encode(response.getProtocolVersion(), status_code,
        response.getHeaders(), body);
    const char* raw_response = parsed_response.c_str();

    auto connection_id = response.getConnectionId();
    _tcp_server.write(connection_id, raw_response, strlen(raw_response));
    _tcp_server.close(connection_id);
}

}
