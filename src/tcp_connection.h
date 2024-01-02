#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include <cstdint>

struct tcp_pcb;
struct pbuf;

namespace __http_internal {

class TcpServer;

class TcpConnection {
public:
    TcpConnection(TcpServer& server,
                  tcp_pcb* pcb):
        _is_closed(false),
        _server(server),
        _pcb(pcb) {
        // Empty on purpose.
    }

    inline uint32_t id() const {
        return reinterpret_cast<uint32_t>(this);
    }

    inline tcp_pcb* getPcb() { return _pcb; }
    inline TcpServer& getServer() { return _server; }

    inline bool isClosed() const { return _is_closed; }
    inline void markAsClosed() { _is_closed = true; }

    bool sink(pbuf* pbuf);
    bool write(const void* data, uint16_t size) const;

    bool flush() const;

private:
    TcpConnection(const TcpConnection& that) = delete;
    TcpConnection& operator=(const TcpConnection& that) = delete;

    tcp_pcb* _pcb;
    TcpServer& _server;

    bool _is_closed;
};

} // namespace __http_internal

#endif // __TCP_CONNECTION_H__
