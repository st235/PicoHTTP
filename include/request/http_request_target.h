#ifndef __HTTP_REQUEST_REQUEST_TARGET_H__
#define __HTTP_REQUEST_REQUEST_TARGET_H__

#include <optional>
#include <string>

#include "authority.h"
#include "http_method.h"
#include "url.h"

namespace {

const std::string kAsterikForm = "*";

} // namespace

namespace http {

namespace __internal {

class RequestTarget {
private:
    enum {
        kAsterikForm,
        kAuthorityForm,
        kUrlForm
    } _form;

    union {
        uri::Url* _url;
        uri::Authority* _authority;
    } _content;

    void copyContent(const RequestTarget& that) {
        switch (that._form) {
            case kAsterikForm:
                _content._url = nullptr;
                break;
            case kAuthorityForm:
                _content._authority = new uri::Authority(*that._content._authority);
                break;
            case kUrlForm:
                _content._url = new uri::Url(*that._content._url);
                break;
        }
    }

    void moveContent(RequestTarget&& that) {
        switch (that._form) {
            case kAsterikForm:
                _content._url = nullptr;
                break;
            case kAuthorityForm:
                _content._authority = new uri::Authority(std::move(*that._content._authority));
                break;
            case kUrlForm:
                _content._url = new uri::Url(std::move(*that._content._url));
                break;
        }
    }

    void deleteContent() {
        switch (_form) {
            case kAsterikForm:
                break;
            case kAuthorityForm:
                delete _content._authority;
                break;
            case kUrlForm:
                delete _content._url;
                break;
        }
    }

public:
    RequestTarget() noexcept:
        _form(kAsterikForm) {
        _content._url = nullptr;
    }

    explicit RequestTarget(const uri::Authority& authority) noexcept:
        _form(kAuthorityForm) {
        _content._authority = new uri::Authority(authority);
    }

    explicit RequestTarget(const uri::Url& url) noexcept:
        _form(kUrlForm) {
        _content._url = new uri::Url(url);
    }

    RequestTarget(const RequestTarget& that) noexcept:
        _form(that._form) {
        copyContent(that);
    }

    RequestTarget& operator=(const RequestTarget& that) noexcept {
        if (this != &that) {
            deleteContent();

            _form = that._form;
            copyContent(that);
        }
        return *this;
    }

    RequestTarget(RequestTarget&& that) noexcept:
        _form(that._form) {
        moveContent(std::move(that));
    }

    RequestTarget& operator=(RequestTarget&& that) noexcept {
        if (this != &that) {
            deleteContent();

            _form = that._form;
            moveContent(std::move(that));
        }
        return *this;
    }

    bool isAsterikForm() const {
        return _form == kAsterikForm;
    }

    bool isAuthorityForm() const {
        return _form == kAuthorityForm;
    }

    bool isUrlForm() const {
        return _form == kUrlForm;
    }

    const uri::Authority& asAuthority() const {
        return *(_content._authority);
    }

    const uri::Url& asUrl() const {
        return *(_content._url);
    }

    ~RequestTarget() {
        deleteContent();
    }
};

static std::optional<RequestTarget> GetRequestTarget(const Method& method,
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

#endif // __HTTP_REQUEST_REQUEST_TARGET_H__
