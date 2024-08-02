#include "request/http_request_target.h"

namespace {

const std::string kAsterikForm = "*";

} // namespace

namespace http {

namespace __internal {

std::optional<RequestTarget> GetRequestTarget(const Method& method,
                                              const std::string& path) {
    if (method == Method::kConnect) {
        // The authority component of a URL,
        // consisting of the domain name and
        // optionally the port (prefixed by a ':'),
        // is called the authority form. It is only
        // used with CONNECT when setting up an HTTP tunnel.
        // CONNECT developer.mozilla.org:80 HTTP/1.1
        std::optional<uri::Authority> opt_authority = uri::Authority::parse(path);
        if (!opt_authority) {
            // Does not conform to authority syntax.
            return std::nullopt;
        }

        return std::make_optional(RequestTarget(opt_authority.value()));
    }

    if (path == kAsterikForm && method != Method::kOptions) {
        return std::nullopt;
    }

    std::optional<uri::Url> opt_url = uri::Url::parse(path);
    if (!opt_url) {
        // Does not conform to uri-reference syntax.
        return std::nullopt;
    }

    return std::make_optional(RequestTarget(opt_url.value()));
}

} // namespace __internal

} // namespace http
