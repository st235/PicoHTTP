#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include <cstdint>
#include <cstring>
#include <string>

#include "tcp_server.h"
#include "http1_1_parser.h"
#include "http_status_code.h"
#include "http_headers.h"

namespace http {

class Response {
public:
    Response(uint32_t connection_id,
                 __internal::TcpServer* tcp_server,
                 const __internal::Http11Parser& parser):
        _connection_id(connection_id),
        _tcp_server(tcp_server),
        _parser(parser),
        _status_code(StatusCode::OK),
        _headers() {
        // Empty on purpose.
    }

    // TODO: add copy and move constructors.

    void setStatusCode(const StatusCode& status_code) {
        _status_code = status_code;
    }

    const StatusCode& getStatusCode() const {
        return _status_code;
    }

    void addHeader(const std::string& key,
                   const std::string& value) {
        _headers[key] = value;
    }

    const Headers& getHeaders() const {
        return _headers;
    }

    void send(const std::string& body) {
        std::string response = _parser.toResponse(*this, body);
        const char* raw_response = response.c_str();

        _tcp_server->write(_connection_id, raw_response, strlen(raw_response));
        _tcp_server->close(_connection_id);
    }

    ~Response() = default;

private:
    uint32_t _connection_id;
    __internal::TcpServer* _tcp_server;
    __internal::Http11Parser _parser;

    StatusCode _status_code;
    Headers _headers;


};

} // namespace http

#endif // __HTTP_RESPONSE_H__
