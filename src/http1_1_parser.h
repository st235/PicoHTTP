#ifndef __HTTP1_1_PARSER_H__
#define __HTTP1_1_PARSER_H__

#include <string>

#include "http_request.h"
#include "http_response.h"

namespace __http_internal {

class Http11Parser {
public:
    http::HttpRequest fromRequest(const std::string& request) const;
    std::string toResponse(const http::HttpResponse& response) const;
};

} // namespace __http_internal

#endif // __HTTP1_1_PARSER_H__
