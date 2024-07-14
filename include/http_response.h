#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include <cstdint>
#include <cstring>
#include <string>

#include "http_protocol_version.h"
#include "http_status_code.h"
#include "http_headers.h"

namespace http {

class Server;

class Response {
public:
    Response(uint32_t connection_id,
             const ProtocolVersion& protocol_version,
             const Server& server):
        _connection_id(connection_id),
        _protocol_version(protocol_version),
        _server(server),
        _headers() {
        // Empty on purpose.
    }

    const ProtocolVersion& getProtocolVersion() const {
        return _protocol_version;
    }

    void addHeader(const std::string& key,
                   const std::string& value) {
        _headers[key] = value;
    }

    const Headers& getHeaders() const {
        return _headers;
    }

    void send(const std::string& body,
              const StatusCode& status_code = StatusCode::kOk) const;

    ~Response() = default;

private:
    friend class Server;

    uint32_t _connection_id;
    const ProtocolVersion& _protocol_version;
    // Response always lives no longer
    // than the server associated
    // with it.
    const Server& _server;
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
