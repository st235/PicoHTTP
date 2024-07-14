#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include <cstdint>
#include <cstring>
#include <string>

#include "http_status_code.h"
#include "http_headers.h"

namespace http {

class Server;

class Response {
public:
    Response(uint32_t connection_id,
            const Server& server):
        _connection_id(connection_id),
        _server(server),
        _status_code(StatusCode::kOk),
        _headers() {
        // Empty on purpose.
    }

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

    void send(const std::string& body) const;

    ~Response() = default;

private:
    friend class Server;

    uint32_t _connection_id;

    // Response always lives no longer
    // than the server associated
    // with it.
    const Server& _server;

    StatusCode _status_code;
    Headers _headers;

    Response(const Response& that) = delete;
    Response& operator=(const Response& that) = delete;
    Response(Response&& that) = delete;
    Response& operator=(Response&& that) = delete;

    inline uint32_t getConnectionId() const {
        return _connection_id;
    }
};

} // namespace http

#endif // __HTTP_RESPONSE_H__
