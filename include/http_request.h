#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <string>
#include <sstream>

#include "http_method.h"
#include "http_headers.h"

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

class HttpRequest {
public:
    HttpRequest(const std::string& http_version,
                const std::string& path,
                HttpMethod method,
                Headers headers,
                const std::unordered_map<std::string, std::string>& query_parameters,
                const std::string& body):
        _http_version(http_version),
        _path(path),
        _method(method),
        _headers(headers),
        _query_parameters(query_parameters),
        _body(body) {
        // Empty on purpose.
    }

    // TODO: add copy and move constructors.

    std::string getHttpVersion() const {
        return _http_version;
    }

    std::string getPath() const {
        return _path;
    }

    http::HttpMethod getMethod() const {
        return _method;
    }

    http::Headers getHeaders() const {
        return _headers;
    }

    bool hasQuery(const std::string& key) const {
        return _query_parameters.find(key) != _query_parameters.end();
    }

    std::string getQuery(const std::string& key) const {
        return _query_parameters.at(key);
    }

    std::string getBody() const {
        return _body;
    }

    std::string toString() const {
        std::stringstream sstream;

        sstream << "HttpRequest{ " << std::endl
                << "version=" << _http_version << ", " << std::endl
                << "path=" << _path << ", " << std::endl
                << "method=" << ConvertHttpMethodToString(_method) << ", " << std::endl
                << "headers=" << _headers.toString() << ", " << std::endl
                << "query_parameters=" << ConvertMapToString(_query_parameters) << ", " << std::endl
                << "body=" << _body << std::endl
                << "}";

        return sstream.str();
    }

    ~HttpRequest() = default;

private:
    std::string _http_version;
    std::string _path;
    HttpMethod _method;
    Headers _headers;
    std::unordered_map<std::string, std::string> _query_parameters;
    std::string _body;
};

} // namespace http

#endif // __HTTP_REQUEST_H__
