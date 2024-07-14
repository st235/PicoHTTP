#include "http_response.h"

#include "http_server.h"

namespace http {

void Response::send(const std::string& body) const {
    _server.send(*this, body);
}

} // namespace http
