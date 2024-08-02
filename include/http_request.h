#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <sstream>
#include <string>

#include "http_headers.h"
#include "http_method.h"
#include "http_protocol_version.h"
#include "request/http_request_target.h"

namespace {

const std::string kEmptyString = "";

} // namespace

namespace http {

class Request {
public:
    using query_parameters_t = std::unordered_map<std::string, std::string>;

    Request(const ProtocolVersion& protocol_version,
            const __internal::RequestTarget& request_target,
            const Method& method,
            const Headers& headers,
            const std::string& body):
        _protocol_version(protocol_version),
        _request_target(request_target),
        _method(method),
        _headers(headers),
        _body(body) {
        // Empty on purpose.
    }

    Request(const Request& that):
        _protocol_version(that._protocol_version),
        _request_target(that._request_target),
        _method(that._method),
        _headers(that._headers),
        _body(that._body) {
        // Empty on purpose.
    }

    Request& operator=(const Request& that) {
        if (this != &that) {
            _protocol_version = that._protocol_version;
            _request_target = that._request_target;
            _method = that._method;
            _headers = that._headers;
            _body = that._body;
        }
        return *this;
    }

    Request(Request&& that):
        _protocol_version(std::move(that._protocol_version)),
        _request_target(std::move(that._request_target)),
        _method(std::move(that._method)),
        _headers(std::move(that._headers)),
        _body(std::move(that._body)) {
        // Empty on purpose.
    }

    Request& operator=(Request&& that) {
        if (this != &that) {
            _protocol_version = std::move(that._protocol_version);
            _request_target = std::move(that._request_target);
            _method = std::move(that._method);
            _headers = std::move(that._headers);
            _body = std::move(that._body);
        }
        return *this;
    }

    const inline ProtocolVersion& getProtocolVersion() const {
        return _protocol_version;
    }

    const std::string& getPath() const {
        if (!_request_target.isUrlForm()) {
            return kEmptyString;
        }

        const auto& url = _request_target.asUrl();
        return url.getPath();
    }

    query_parameters_t getQuery() const {
        if (!_request_target.isUrlForm()) {
            return query_parameters_t();
        }

        const auto& url = _request_target.asUrl();
        return url.getQuery();
    }

    const inline http::Method& getMethod() const {
        return _method;
    }

    const inline http::Headers& getHeaders() const {
        return _headers;
    }

    bool hasQuery(const std::string& key) const {
        if (!_request_target.isUrlForm()) {
            return false;
        }

        const auto& url = _request_target.asUrl();
        return url.getQuery().find(key) != url.getQuery().end();
    }

    const std::string& getQuery(const std::string& key) const {
        const auto& url = _request_target.asUrl();
        return url.getQuery().at(key);
    }

    const inline std::string& getBody() const {
        return _body;
    }

    std::string toString() const {
        // No-Op.
        // TODO(st235): remove.
        return "";
    }

    ~Request() = default;

private:
    ProtocolVersion _protocol_version;
    __internal::RequestTarget _request_target;
    Method _method;
    Headers _headers;
    std::string _body;
};

} // namespace http

#endif // __HTTP_REQUEST_H__
