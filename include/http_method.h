#ifndef __HTTP_METHOD_H__
#define __HTTP_METHOD_H__

#include <string>

namespace http {

enum class Method {
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

inline std::string ConvertHttpMethodToString(const Method& method) {
    switch (method) {
        case Method::GET: return std::string("GET");
        case Method::HEAD: return std::string("HEAD");
        case Method::POST: return std::string("POST");
        case Method::PUT: return std::string("PUT");
        case Method::DELETE: return std::string("DELETE");
        case Method::CONNECT: return std::string("CONNECT");
        case Method::OPTIONS: return std::string("OPTIONS");
        case Method::TRACE: return std::string("TRACE");
        case Method::PATCH: return std::string("PATCH");
        default: return std::string("UNKNOWN");
    }
}

} // namespace http

#endif // __HTTP_METHOD_H__
