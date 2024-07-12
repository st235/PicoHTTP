#ifndef __HTTP_METHOD_H__
#define __HTTP_METHOD_H__

#include <string>

namespace http {

// Http Request methods.
// See https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods
// for more info.
enum class Method {
    kGet,
    kHead,
    kPost,
    kPut,
    kDelete,
    kConnect,
    kOptions,
    kTrace,
    kPatch,
};

inline std::string ConvertHttpMethodToString(const Method& method) {
    switch (method) {
        case Method::kGet: return std::string("GET");
        case Method::kHead: return std::string("HEAD");
        case Method::kPost: return std::string("POST");
        case Method::kPut: return std::string("PUT");
        case Method::kDelete: return std::string("DELETE");
        case Method::kConnect: return std::string("CONNECT");
        case Method::kOptions: return std::string("OPTIONS");
        case Method::kTrace: return std::string("TRACE");
        case Method::kPatch: return std::string("PATCH");
        default: return std::string("UNKNOWN");
    }
}

} // namespace http

#endif // __HTTP_METHOD_H__
