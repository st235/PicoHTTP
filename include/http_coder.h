#ifndef __HTTP_CODER_H__
#define __HTTP_CODER_H__

#include <string>
#include <optional>

#include "http_headers.h"
#include "http_protocol_version.h"
#include "http_status_code.h"

namespace http {

class Request;

namespace __internal {

class Coder {
  public:
    std::optional<Request> decode(const std::string& request) const;
    std::string encode(const ProtocolVersion& protocol_version,
                       const StatusCode& status_code,
                       const Headers& headers,
                       const std::string& body) const;
};

} // namespace __internal

} // namespace http

#endif // __HTTP_CODER_H__
