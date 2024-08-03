#include "http_response.h"

#include "http_server.h"

namespace http {

void Response::send(const std::string& body,
                    const StatusCode& status_code) const {
    _server.send(*this, body, status_code);
}

} // namespace http
