#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "tcp_connection.h"

struct tcp_pcb;

namespace __http_internal {

class TcpServer {
public:
    typedef void(*OnConnectedCallback)(uint32_t connection_id);
    typedef bool(*OnDataReceivedCallback)(uint32_t connection_id, uint8_t* data, uint16_t size);
    typedef void(*OnClosedCallback)(uint32_t connection_id);

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
    void onClose(TcpConnection* connection);

    bool listen(uint16_t port);
    bool stop();

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

} // __http_internal

#endif // __TCP_SERVER_H__
