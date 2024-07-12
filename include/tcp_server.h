#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <cstdint>
#include <memory>
#include <functional>
#include <unordered_map>

#include "tcp_connection.h"

struct tcp_pcb;

namespace tcp {

class TcpServer {
public:
    typedef std::function<void(uint32_t connection_id)> OnConnectedCallback;
    typedef std::function<bool(uint32_t connection_id, uint8_t* data, uint16_t size)> OnDataReceivedCallback;
    typedef std::function<void(uint32_t connection_id)> OnClosedCallback;

    TcpServer(uint8_t max_connections):
        _max_connections(max_connections) {
        // Empty on purpose.
    }

    inline void setOnConnectedCallback(OnConnectedCallback callback) {
        _onConnectedCallback = callback;
    }

    inline void setOnDataReceivedCallback(OnDataReceivedCallback callback) {
        _onDataReceivedCallback = callback;
    }

    inline void setOnClosedCallback(OnClosedCallback callback) {
        _onClosedCallback = callback;
    }

    TcpConnection* createConnection(tcp_pcb* pcb);

    void onConnected(TcpConnection* connection);
    bool onDataReceived(TcpConnection* connection, uint8_t* data, uint16_t size);
    void onConnectionClosed(TcpConnection* connection);

    bool write(uint32_t connection_id, const void* data, uint16_t size) const;
    void close(uint32_t connection_id) const;

    bool listen(uint16_t port);
    bool stop();

    ~TcpServer() = default;

private:
    TcpServer(const TcpServer& that) = delete;
    TcpServer& operator=(const TcpServer& that) = delete;

    // TODO: add destructor to clear resources and stop listeners.

    inline TcpConnection* findConnectionById(uint32_t connection_id) const {
        if (_connections.find(connection_id) == _connections.end()) {
            return nullptr;
        }
        return _connections.at(connection_id).get();
    }

    uint8_t _max_connections;
    tcp_pcb* _listen_pcb = nullptr;
    std::unordered_map<uint32_t, std::unique_ptr<TcpConnection>> _connections;

    OnConnectedCallback _onConnectedCallback = nullptr;
    OnDataReceivedCallback _onDataReceivedCallback = nullptr;
    OnClosedCallback _onClosedCallback = nullptr;
};

} // namespace tcp

#endif // __TCP_SERVER_H__
