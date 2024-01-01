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
        if (this != &that) {
            this->_headers = that._headers;
        }
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
        for (auto kv_pair: _headers) {
            keys.push_back(kv_pair.first);
        }
        return keys;
    }

    inline bool contains(const std::string& key) const {
        return _headers.find(key) != _headers.end();
    }

    bool put(const std::string& key, const std::string& value) {
        if (contains(key)) {
            return false;
        }

        _headers[key] = value;
        return true;
    }

    inline std::string get(const std::string& key) const {
        return _headers.at(key);
    }

    std::string operator[](const std::string& key) const { 
        return get(key);
    }

    std::string& operator[](const std::string& key) { 
        return _headers[key];
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
