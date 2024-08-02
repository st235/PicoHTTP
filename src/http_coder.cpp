#include "http_coder.h"

#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "url.h"

#include "http_headers.h"
#include "http_method.h"
#include "http_protocol_version.h"
#include "http_request.h"
#include "http_utils.h"
#include "string_utils.h"
#include "request/http_request_target.h"

namespace {

constexpr char kNewLine = '\n';

constexpr char kHttpNewLine[] = "\r\n";
constexpr char kHttpWordsDelimiter = ' ';

} // namespace

namespace http {

namespace __internal {

std::optional<Request> Coder::decode(const std::string& request) const {
    std::vector<std::string> requst_split =
        Split(request, /* delimiter= */ std::string(kHttpNewLine));

    std::string start_line = requst_split[0];
    std::vector<std::string> start_line_split =
        Split(start_line, /* delimiter= */ std::string(" "));

    // TODO(st235): check for method here.
    http::Method method = ConvertStringToHttpMethod(start_line_split[0]);

    std::string raw_request_target = start_line_split[1];
    std::optional<RequestTarget> opt_request_target = GetRequestTarget(method, raw_request_target);
    if (!opt_request_target) {
        return std::nullopt;
    }

    // TODO(st235): Create mapper for protocol version.
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

    return std::make_optional(Request(ProtocolVersion::kHttp1_1, opt_request_target.value(), method, headers, Trim(body.str())));
}

std::string Coder::encode(const ProtocolVersion& protocol_version,
                          const StatusCode& status_code,
                          const Headers& headers,
                          const std::string& body) const {
    std::stringstream sstream;

    // Main line: HTTP/1.1 403 Forbidden
    sstream << ConvertProtocolVersionToString(protocol_version) << kHttpWordsDelimiter
            << GetHttpStatusCodeDescription(status_code) << kHttpNewLine;

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
