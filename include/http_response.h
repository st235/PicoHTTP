#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include <string>

#include "http_status_code.h"
#include "http_headers.h"

namespace http {

class HttpResponse {
public:
    HttpResponse(HttpStatusCode status_code,
                 HttpHeaders headers,
                 const std::string& body):
        _status_code(status_code),
        _headers(headers),
        _body(body) {
        // Empty on purpose.
    }

    // TODO: add copy and move constructors.

    // TODO: addHeader method.

    const HttpStatusCode& getStatusCode() const {
        return _status_code;
    }

    const HttpHeaders& getHeaders() const {
        return _headers;
    }

    const std::string& getBody() const {
        return _body;
    }

private:
    HttpStatusCode _status_code;
    HttpHeaders _headers;
    std::string _body;
};

} // namespace http

#endif // __HTTP_RESPONSE_H__
