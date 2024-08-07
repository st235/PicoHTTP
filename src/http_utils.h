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
        return http::Method::kGet;
    } else if (method == "HEAD") {
        return http::Method::kHead;
    } else if (method == "POST") {
        return http::Method::kPost;
    } else if (method == "PUT") {
        return http::Method::kPut;
    } else if (method == "DELETE") {
        return http::Method::kDelete;
    } else if (method == "CONNECT") {
        return http::Method::kConnect;
    } else if (method == "OPTIONS") {
        return http::Method::kOptions;
    } else if (method == "TRACE") {
        return http::Method::kTrace;
    } else if (method == "PATCH") {
        return http::Method::kPatch;
    } else {
        // TODO: return some unknown type.
        return http::Method::kGet;
    }
}

std::string GetHttpStatusCodeDescription(const http::StatusCode& status_code) {
    switch(status_code) {
        case http::StatusCode::kOk: return std::string("200 OK");
        case http::StatusCode::kBadRequest: return std::string("400 Bad Request");
        case http::StatusCode::kUnauthorized: return std::string("401 Unauthorized");
        case http::StatusCode::kForbidden: return std::string("403 Forbidden");
        case http::StatusCode::kNotFound: return std::string("404 Not Found");
        case http::StatusCode::kInternalServerError: return std::string("500 Internal Server Error");
        // TODO(st235): add all conversions.
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