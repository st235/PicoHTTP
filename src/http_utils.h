#ifndef __HTTP_UTILS_H__
#define __HTTP_UTILS_H__

#include <string>
#include <unordered_map>

#include "http_method.h"
#include "http_status_code.h"
#include "string_utils.h"

namespace {

bool ParseKeyValueStatement(const std::string& statement,
                            char delimiter,
                            std::string& key,
                            std::string& value) {
    size_t delimiter_position = statement.find(delimiter);

    if (delimiter_position == std::string::npos) {
        return false;
    }

    key = http::__internal::Trim(statement.substr(0, delimiter_position));
    value = http::__internal::Trim(statement.substr(delimiter_position + 1, statement.length() - delimiter_position - 1));

    return !key.empty() && !value.empty();
}

}  // namespace

namespace http {

namespace __internal {

http::Method ConvertStringToHttpMethod(const std::string& method) {
    if (method == "GET") {
        return http::Method::GET;
    } else if (method == "HEAD") {
        return http::Method::HEAD;
    } else if (method == "POST") {
        return http::Method::POST;
    } else if (method == "PUT") {
        return http::Method::PUT;
    } else if (method == "DELETE") {
        return http::Method::DELETE;
    } else if (method == "CONNECT") {
        return http::Method::CONNECT;
    } else if (method == "OPTIONS") {
        return http::Method::OPTIONS;
    } else if (method == "TRACE") {
        return http::Method::TRACE;
    } else if (method == "PATCH") {
        return http::Method::PATCH;
    } else {
        // TODO: return some unknown type.
        return http::Method::GET;
    }
}

std::string GetHttpStatusCodeDescription(const http::StatusCode& status_code) {
    switch(status_code) {
        case http::StatusCode::OK: return std::string("200 OK");
        case http::StatusCode::BAD_REQUEST: return std::string("400 Bad Request");
        case http::StatusCode::UNAUTHORIZED: return std::string("401 Unauthorized");
        case http::StatusCode::FORBIDDEN: return std::string("403 Forbidden");
        case http::StatusCode::NOT_FOUND: return std::string("404 Not Found");
        case http::StatusCode::INTERNAL_SERVER_ERROR: return std::string("500 Internal Server Error");
        default: return std::string("418 I'm a teapot");
    }
}

std::string GetRoute(const std::string& path) {
    size_t delimiter_position = path.find('?');

    if (delimiter_position == std::string::npos) {
        return path;
    }

    return path.substr(0, delimiter_position);
}

std::unordered_map<std::string, std::string> ParseQueryParameters(const std::string& path) {
    size_t delimiter_position = path.find('?');

    if (delimiter_position == std::string::npos) {
        return std::unordered_map<std::string, std::string>();
    }

    std::unordered_map<std::string, std::string> result;
    std::string query_params = path.substr(delimiter_position + 1, path.length() - delimiter_position - 1);
    std::vector<std::string> query_splits = Split(query_params, /* delimiter= */ "&");

    for (const auto& query: query_splits) {
        std::string key;
        std::string value;

        if (ParseKeyValueStatement(query, /* delimiter= */ '=', key, value)) {
            result[key] = value;
        }
    }

    return result;
}

bool ParseSingleHeaderLine(const std::string& header,
                           std::string& key,
                           std::string& value) {
    return ParseKeyValueStatement(header, /* delimiter= */ ':', key, value);
}

} // namespace __internal

} // namespace http

#endif // __HTTP_UTILS_H__