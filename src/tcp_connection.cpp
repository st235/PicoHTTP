#include "tcp_connection.h"

#include "lwip/tcp.h"

#include "log.h"
#include "tcp_server.h"

namespace __http_internal {

bool TcpConnection::sink(pbuf* pbuf) {
    uint8_t* data = new uint8_t[pbuf->tot_len];
    for (uint16_t i = 0; i < pbuf->tot_len; i++) {
        data[i] = pbuf_get_at(pbuf, i);
    }

    auto sink_status = _server.onDataReceived(this, data, pbuf->tot_len);

    // Delete data allocated in heap.
    delete[] data;

    return sink_status;
}

bool TcpConnection::write(const void* data, uint16_t size) const {
    if (_is_closing) {
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
    if (_is_closing) {
        PLOGD("Accessing closed connection.");
        return false;
    }

    return tcp_output(_pcb) == ERR_OK;
}

} // namespace __http_internal
