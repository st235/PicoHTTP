#ifndef __HTTP1_1_PARSER_H__
#define __HTTP1_1_PARSER_H__

#include <string>

namespace http {

class Request;
class HttpResponse;

namespace __internal {

class Http11Parser {
public:
    http::Request fromRequest(const std::string& request) const;
    std::string toResponse(const http::HttpResponse& response, const std::string& body) const;
};

} // namespace __internal

} // namespace http

#endif // __HTTP1_1_PARSER_H__
