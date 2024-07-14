#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "tcp_server.h"
#include "http_method.h"
#include "http_request.h"
#include "http_response.h"

namespace {

constexpr uint8_t kDefaultMaxConnection = 8;

} // namespace

namespace http {

class Server {
  public:
    typedef std::function<void(const Request& request, Response& response)> OnRouteCallback;

    Server(uint16_t port,
           uint8_t max_connections = kDefaultMaxConnection);

    inline void onGet(const std::string& route,
                      OnRouteCallback callback) {
      onMethod(Method::kGet, route, callback);
    }

    inline void onPost(const std::string& route,
                       OnRouteCallback callback) {
      onMethod(Method::kPost, route, callback);
    }

    void start();

    ~Server() = default;

  private:
    friend class Response;

    uint16_t _port;
    uint8_t _max_connections;
    tcp::Server _tcp_server;
    std::unordered_map<Method, std::unordered_map<std::string, OnRouteCallback>> _routes;

    Server(const Server& that) = delete;
    Server& operator=(const Server& that) = delete;
    Server(Server&& that) = delete;
    Server& operator=(Server&& that) = delete;

    const OnRouteCallback* findRouteCallback(const Method& method,
                                       const std::string& route) const;

    void onMethod(const Method& method,
                  const std::string& route,
                  OnRouteCallback callback);

    void send(const Response& response,
              const std::string& body) const;
};

}

#endif // __HTTP_SERVER_H__
