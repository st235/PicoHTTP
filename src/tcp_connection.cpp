#include "tcp_connection.h"

#include "lwip/tcp.h"

#include "log.h"
#include "tcp_server.h"

namespace __http_internal {

bool TcpConnection::sink(pbuf* pbuf) {
    char* result = new char[pbuf->tot_len];
    for (uint16_t i = 0; i < pbuf->tot_len; i++) {
        result[i] = pbuf_get_at(pbuf, i);
    }
    return _server.onDataReceived(this, result, pbuf->tot_len);
}

bool TcpConnection::write(const void* data, uint16_t size) const {
    if (_is_closed) {
        PLOGD("Accessing closed connection.");
        return false;
    }

    if (data == nullptr && size > 0) {
        PLOGD("Data was empty but size was %d.", size);
        return false;
    }

    if (size == 0) {
        // No data to write
        return true;
    }

    return tcp_write(_pcb, data, size, /* apiflags= */ TCP_WRITE_FLAG_COPY) == ERR_OK;
}

bool TcpConnection::flush() const {
    if (_is_closed) {
        PLOGD("Accessing closed connection.");
        return false;
    }

    return tcp_output(_pcb) == ERR_OK;
}

bool TcpConnection::close() {
    _is_closed = true;
    _server.onClose(this);
    return tcp_close(_pcb) == ERR_OK;
}

} // namespace __http_internal
