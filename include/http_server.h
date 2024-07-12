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
      onMethod(Method::GET, route, callback);
    }

    inline void onPost(const std::string& route,
                       OnRouteCallback callback) {
      onMethod(Method::POST, route, callback);
    }

    void start();

    ~Server() = default;

  private:
    const OnRouteCallback* findRouteCallback(const Method& method,
                                       const std::string& route) const;

    void onMethod(const Method& method,
                  const std::string& route,
                  OnRouteCallback callback);

    uint16_t _port;
    uint8_t _max_connections;
    std::unique_ptr<__internal::TcpServer> _tcp_server;
    std::unordered_map<Method, std::unordered_map<std::string, OnRouteCallback>> _routes;
};

}

#endif // __HTTP_SERVER_H__
