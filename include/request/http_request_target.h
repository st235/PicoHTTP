#ifndef __HTTP_REQUEST_REQUEST_TARGET_H__
#define __HTTP_REQUEST_REQUEST_TARGET_H__

#include <string>
#include <optional>

#include "authority.h"
#include "http_method.h"
#include "url.h"

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

std::optional<RequestTarget> GetRequestTarget(const Method& method,
                                              const std::string& path);

} // namespace __internal

} // namespace http

#endif // __HTTP_REQUEST_REQUEST_TARGET_H__
