#ifndef __HTTP_METHOD_H__
#define __HTTP_METHOD_H__

#include <string>

namespace http {

enum class HttpMethod {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
};

inline std::string ConvertHttpMethodToString(const HttpMethod& method) {
    switch (method) {
        case HttpMethod::GET: return std::string("GET");
        case HttpMethod::HEAD: return std::string("HEAD");
        case HttpMethod::POST: return std::string("POST");
        case HttpMethod::PUT: return std::string("PUT");
        case HttpMethod::DELETE: return std::string("DELETE");
        case HttpMethod::CONNECT: return std::string("CONNECT");
        case HttpMethod::OPTIONS: return std::string("OPTIONS");
        case HttpMethod::TRACE: return std::string("TRACE");
        case HttpMethod::PATCH: return std::string("PATCH");
        default: return std::string("UNKNOWN");
    }
}

} // namespace http

#endif // __HTTP_METHOD_H__
