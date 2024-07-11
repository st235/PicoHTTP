#ifndef __HTTP_HEADERS_H__
#define __HTTP_HEADERS_H__

#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace http {

class HttpHeaders {
public:
    HttpHeaders() = default;

    HttpHeaders(const HttpHeaders& that): _headers() {
        this->_headers = that._headers;
    }

    HttpHeaders& operator=(const HttpHeaders& that) {
        if (this != &that) {
            this->_headers.clear();
            this->_headers = that._headers;
        }
        return *this;
    }

    HttpHeaders(HttpHeaders&& that) {
        this->_headers = std::move(that._headers);
    }

    HttpHeaders& operator=(HttpHeaders&& that) {
        if (this != &that) {
            this->_headers = std::move(that._headers);
        }
        return *this;
    }

    std::vector<std::string> keys() const {
        std::vector<std::string> keys;
        keys.reserve(_headers.size());
        for (const auto& [key, _]: _headers) {
            keys.push_back(key);
        }
        return keys;
    }

    std::string& operator[](const std::string& key) { 
        return _headers[key];
    }

    const std::string& operator[](const std::string& key) const { 
        return _headers.at(key);
    }

    inline bool contains(const std::string& key) const {
        return _headers.find(key) != _headers.end();
    }

    size_t size() const {
        return _headers.size();
    }

    std::string toString() const {
        std::stringstream sstream;

        sstream << "HttpHeaders{headers=[" << std::endl;

        for (auto i = _headers.begin(); i != _headers.end(); i++) {
            if (i != _headers.begin()) {
                sstream << ", " << std::endl;
            }

            sstream << i->first << "=" << i->second;
        }

        sstream << "]}";
        return sstream.str();
    }

private:
    std::unordered_map<std::string, std::string> _headers;
};

} // namespace http

#endif // __HTTP_HEADERS_H__
