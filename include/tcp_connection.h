#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include <cstdint>
#include <cstddef>

struct tcp_pcb;
struct pbuf;

namespace tcp {

class Server;

class Connection {
public:
    Connection(Server& server,
               tcp_pcb& pcb):
        _server(server),
        _pcb(pcb),
        _is_closing(false) {
        // Empty on purpose.
    }

    inline size_t id() const {
        return reinterpret_cast<size_t>(this);
    }

    inline tcp_pcb& getPcb() const { return _pcb; }
    inline Server& getServer() const { return _server; }

    inline void close() { _is_closing = true; }
    inline bool isClosing() const { return _is_closing; }

    bool sink(pbuf* pbuf);
    bool write(const void* data, uint16_t size) const;

    bool flush() const;

private:
    // Connection does not own neither server nor pcb,
    // connection just observes them and interacts.
    Server& _server;
    tcp_pcb& _pcb;

    bool _is_closing;

    // Connection cannot be copied or moved,
    // only non-owning raw pointer can be passed.
    Connection(const Connection& that) = delete;
    Connection& operator=(const Connection& that) = delete;
    Connection(Connection&& that) = delete;
    Connection& operator=(Connection&& that) = delete;
};

} // namespace tcp

#endif // __TCP_CONNECTION_H__
