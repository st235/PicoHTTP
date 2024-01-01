#ifndef __HTTP_UTILS_H__
#define __HTTP_UTILS_H__

#include <string>
#include <unordered_map>

#include "http_method.h"
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

    key = __http_internal::Trim(statement.substr(0, delimiter_position));
    value = __http_internal::Trim(statement.substr(delimiter_position + 1, statement.length() - delimiter_position - 1));

    return !key.empty() && !value.empty();
}

}  // namespace

namespace __http_internal {

http::HttpMethod ConvertStringToHttpMethod(const std::string& method) {
    if (method == "GET") {
        return http::HttpMethod::GET;
    } else if (method == "HEAD") {
        return http::HttpMethod::HEAD;
    } else if (method == "POST") {
        return http::HttpMethod::POST;
    } else if (method == "PUT") {
        return http::HttpMethod::PUT;
    } else if (method == "DELETE") {
        return http::HttpMethod::DELETE;
    } else if (method == "CONNECT") {
        return http::HttpMethod::CONNECT;
    } else if (method == "OPTIONS") {
        return http::HttpMethod::OPTIONS;
    } else if (method == "TRACE") {
        return http::HttpMethod::TRACE;
    } else if (method == "PATCH") {
        return http::HttpMethod::PATCH;
    } else {
        // TODO: return some unknown type.
        return http::HttpMethod::GET;
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

} // namespace __http_internal

#endif // __HTTP_UTILS_H__