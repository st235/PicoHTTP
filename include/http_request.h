#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <sstream>
#include <string>

#include "http_headers.h"
#include "http_method.h"
#include "http_protocol_version.h"

namespace {

template<class K, class V>
std::string ConvertMapToString(const std::unordered_map<K, V>& map) {
    std::stringstream sstream;

    sstream << '[';

    for (auto i = map.begin(); i != map.end(); i++) {
        if (i != map.begin()) {
            sstream << ", ";
        }
        sstream << i->first << '=' << i->second;
    }

    sstream << "]";
    return sstream.str();
}

} // namespace

namespace http {

class Request {
public:
    Request(const ProtocolVersion& protocol_version,
            const std::string& path,
            const Method& method,
            const Headers& headers,
            const std::unordered_map<std::string, std::string>& query_parameters,
            const std::string& body):
        _protocol_version(protocol_version),
        _path(path),
        _method(method),
        _headers(headers),
        _query_parameters(query_parameters),
        _body(body) {
        // Empty on purpose.
    }

    Request(const Request& that):
        _protocol_version(that._protocol_version),
        _path(that._path),
        _method(that._method),
        _headers(that._headers),
        _query_parameters(that._query_parameters),
        _body(that._body) {
        // Empty on purpose.
    }

    Request& operator=(const Request& that) {
        if (this != &that) {
            _protocol_version = that._protocol_version;
            _path = that._path;
            _method = that._method;
            _headers = that._headers;
            _query_parameters = that._query_parameters;
            _body = that._body;
        }
        return *this;
    }

    Request(Request&& that):
        _protocol_version(std::move(that._protocol_version)),
        _path(std::move(that._path)),
        _method(std::move(that._method)),
        _headers(std::move(that._headers)),
        _query_parameters(std::move(that._query_parameters)),
        _body(std::move(that._body)) {
        // Empty on purpose.
    }

    Request& operator=(Request&& that) {
        if (this != &that) {
            _protocol_version = std::move(that._protocol_version);
            _path = std::move(that._path);
            _method = std::move(that._method);
            _headers = std::move(that._headers);
            _query_parameters = std::move(that._query_parameters);
            _body = std::move(that._body);
        }
        return *this;
    }

    const ProtocolVersion& getProtocolVersion() const {
        return _protocol_version;
    }

    const std::string& getPath() const {
        return _path;
    }

    const http::Method& getMethod() const {
        return _method;
    }

    const http::Headers& getHeaders() const {
        return _headers;
    }

    bool hasQuery(const std::string& key) const {
        return _query_parameters.find(key) != _query_parameters.end();
    }

    const std::string& getQuery(const std::string& key) const {
        return _query_parameters.at(key);
    }

    const std::string& getBody() const {
        return _body;
    }

    std::string toString() const {
        std::stringstream sstream;

        sstream << "Request{ " << std::endl
                // << "version=" << _protocol_version << ", " << std::endl
                << "path=" << _path << ", " << std::endl
                << "method=" << ConvertHttpMethodToString(_method) << ", " << std::endl
                << "headers=" << _headers.toString() << ", " << std::endl
                << "query_parameters=" << ConvertMapToString(_query_parameters) << ", " << std::endl
                << "body=" << _body << std::endl
                << "}";

        return sstream.str();
    }

    ~Request() = default;

private:
    ProtocolVersion _protocol_version;
    std::string _path;
    Method _method;
    Headers _headers;
    std::unordered_map<std::string, std::string> _query_parameters;
    std::string _body;
};

} // namespace http

#endif // __HTTP_REQUEST_H__
