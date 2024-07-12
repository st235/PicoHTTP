#include "http1_1_parser.h"

#include <sstream>
#include <vector>
#include <unordered_map>

#include "http_version.h"
#include "http_method.h"
#include "http_headers.h"
#include "http_request.h"
#include "http_response.h"
#include "http_utils.h"
#include "string_utils.h"

namespace {

constexpr char kNewLine = '\n';

constexpr char kHttpNewLine[] = "\r\n";
constexpr char kHttpWordsDelimiter = ' ';

} // namespace

namespace http {

namespace __internal {

http::Request Http11Parser::fromRequest(const std::string& request) const {
    std::vector<std::string> requst_split = Split(request, /* delimiter= */ std::string(kHttpNewLine));

    std::string start_line = requst_split[0];
    std::vector<std::string> start_line_split = Split(start_line, /* delimiter= */ std::string(" "));

    std::string raw_route = start_line_split[1];
    std::string route = GetRoute(raw_route);
    std::unordered_map<std::string, std::string> query_parameters = ParseQueryParameters(raw_route);

    http::Method http_method = ConvertStringToHttpMethod(start_line_split[0]);
    std::string http_version = start_line_split[2];

    http::Headers headers;

    size_t headers_line = 1;
    while (headers_line < requst_split.size()) {
        std::string header_candidate = requst_split[headers_line];

        if (header_candidate.empty()) {
            break;
        }

        std::string key;
        std::string value;
        if (ParseSingleHeaderLine(header_candidate, key, value)) {
            headers[key] = value;
        }

        headers_line++;
    }

    std::stringstream body;
    while (headers_line < requst_split.size()) {
        body << requst_split[headers_line] << kNewLine;
        headers_line++;
    }

    return http::Request(http_version, route, http_method, headers, query_parameters, Trim(body.str()));
}

std::string Http11Parser::toResponse(const http::Response& response, const std::string& body) const {
    std::stringstream sstream;

    // Main line: HTTP/1.1 403 Forbidden
    sstream << kHttp11Version << kHttpWordsDelimiter 
            << GetHttpStatusCodeDescription(response.getStatusCode()) << kHttpNewLine;

    const auto& headers = response.getHeaders();

    for (const auto& header_key: headers.keys()) {
        sstream << header_key << ':' << headers[header_key] << kHttpNewLine;
    }

    // New line to separate body.
    sstream << kHttpNewLine;

    sstream << body << kHttpNewLine;
    return sstream.str();
}

} // namespace __internal

} // namespace http
