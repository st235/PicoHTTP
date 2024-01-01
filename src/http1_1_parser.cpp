#include "http1_1_parser.h"

#include <sstream>
#include <vector>
#include <unordered_map>

#include "http_method.h"
#include "http_headers.h"
#include "http_utils.h"
#include "string_utils.h"

namespace {

constexpr char kNewLine = '\n';
constexpr char kHttpNewLine[] = "\r\n";

} // namespace

namespace __http_internal {

http::HttpRequest Http11Parser::fromRequest(const std::string& request) const {
    std::vector<std::string> requst_split = Split(request, /* delimiter= */ std::string(kHttpNewLine));

    std::string start_line = requst_split[0];
    std::vector<std::string> start_line_split = Split(start_line, /* delimiter= */ std::string(" "));

    std::string raw_route = start_line_split[1];
    std::string route = GetRoute(raw_route);
    std::unordered_map<std::string, std::string> query_parameters = ParseQueryParameters(raw_route);

    http::HttpMethod http_method = ConvertStringToHttpMethod(start_line_split[0]);
    std::string http_version = start_line_split[2];

    http::HttpHeaders headers;

    size_t headers_line = 1;
    while (headers_line < requst_split.size()) {
        std::string header_candidate = requst_split[headers_line];

        if (header_candidate.empty()) {
            break;
        }

        std::string key;
        std::string value;
        if (ParseSingleHeaderLine(header_candidate, key, value)) {
            headers.put(key, value);
        }

        headers_line++;
    }

    std::stringstream body;
    while (headers_line < requst_split.size()) {
        body << requst_split[headers_line] << kNewLine;
        headers_line++;
    }

    return http::HttpRequest(http_version, route, http_method, headers, query_parameters, Trim(body.str()));
}

} // namespace __http_internal
