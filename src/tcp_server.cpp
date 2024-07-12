#include "tcp_server.h"

#include "lwip/tcp.h"
#include "pico/cyw43_arch.h"

#include "log.h"

namespace {

constexpr uint8_t kTcpNoPoll = 0;
constexpr uint8_t kTcpPollInterval = 5;

tcp_pcb* create_listen_pcb(uint16_t port, 
                           void* argument) {
    cyw43_arch_lwip_check();

    tcp_pcb* temp_pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!temp_pcb) {
        PLOGD("Cannot create new ip type.");
        return nullptr;
    }

    if (tcp_bind(temp_pcb, IP_ADDR_ANY, port) != ERR_OK) {
        PLOGD("Cannot bind temp_pcb.");
        tcp_abort(temp_pcb);
        return nullptr;
    }

    tcp_pcb* listen_pcb = tcp_listen(temp_pcb);

    if (!listen_pcb) {
        PLOGD("Cannot create listen_pcb.");
        tcp_abort(temp_pcb);
    }

    tcp_arg(listen_pcb, argument);

    // temp_pcb is deallocated in tcp_listen.
    return listen_pcb;
}

err_t on_receive_data(void* argument,
                      tcp_pcb* pcb,
                      pbuf* pbuf,
                      err_t err) {
    cyw43_arch_lwip_check();
    auto* connection = static_cast<tcp::TcpConnection*>(argument);

    if (!connection) {
        PLOGD("No connection found on receiving data.");
        if (pbuf) {
            PLOGD("No connection, but pbuf is not empty.");
            pbuf_free(pbuf);
        }
        return ERR_OK;
    }

    // Assuming there is no pbuf data,
    // we want to close the connection.
    bool keep_alive = false;
    if (pbuf) {
        keep_alive = connection->sink(pbuf);
        tcp_recved(pcb, pbuf->tot_len);
        pbuf_free(pbuf);
    }

    if (!keep_alive) {
        tcp_arg(pcb, nullptr);
        tcp_recv(pcb, nullptr);
        tcp_poll(pcb, nullptr, kTcpNoPoll);
        tcp_err(pcb, nullptr);
        // Mark connection as closed.
        connection->close();

        // Synchronise the connection state
        // with possible observers.
        auto& server = connection->getServer();
        server.onConnectionClosed(connection);

        if (tcp_close(pcb) != ERR_OK) {
            PLOGD("Connection closed with an error, aboring with tcp_abort.");
            tcp_abort(pcb);
            return ERR_ABRT;
        }
    }

    return ERR_OK;
}

err_t on_poll(void* argument, 
              tcp_pcb* pcb) {
    cyw43_arch_lwip_check();

    auto* connection = static_cast<tcp::TcpConnection*>(argument);

    if (!connection) {
        PLOGD("Argument was null");
        if (pcb) {
            PLOGD("Aborting as pcb is available.");
            tcp_abort(pcb);
            return ERR_ABRT;
        }
    }

    // Connection has been marked as closed
    // from the client clode.
    if (connection->isClosing() && pcb) {
        PLOGD("Closing stale connection.");
        tcp_arg(pcb, nullptr);
        tcp_recv(pcb, nullptr);
        tcp_poll(pcb, nullptr, kTcpNoPoll);
        tcp_sent(pcb, nullptr);
        tcp_err(pcb, nullptr);

        // Triggering the callback.
        auto& server = connection->getServer();
        server.onConnectionClosed(connection);

        if (tcp_close(pcb) != ERR_OK) {
            PLOGD("Connection closed with an error, aboring with tcp_abort.");
            tcp_abort(pcb);
            return ERR_ABRT;
        }
    }

    return ERR_OK;
}

err_t on_sent(void* argument,
             tcp_pcb* pcb,
             u16_t len) {
    cyw43_arch_lwip_check();

    auto* connection = static_cast<tcp::TcpConnection*>(argument);

    if (!connection) {
        PLOGD("Argument was null");
        if (pcb) {
            PLOGD("Aborting as pcb is available.");
            tcp_abort(pcb);
            return ERR_ABRT;
        }
    }

    // Connection has been marked as closed
    // from the client clode.
    if (connection->isClosing() && pcb) {
        PLOGD("Closing stale connection.");
        tcp_arg(pcb, nullptr);
        tcp_recv(pcb, nullptr);
        tcp_poll(pcb, nullptr, kTcpNoPoll);
        tcp_sent(pcb, nullptr);
        tcp_err(pcb, nullptr);

        // Triggering the callback.
        auto& server = connection->getServer();
        server.onConnectionClosed(connection);

        if (tcp_close(pcb) != ERR_OK) {
            PLOGD("Connection closed with an error, aboring with tcp_abort.");
            tcp_abort(pcb);
            return ERR_ABRT;
        }
    }

    return ERR_OK;
}

void on_error(void* argument,
              err_t error) {
    cyw43_arch_lwip_check();

    if (error != ERR_ABRT) {
        PLOGD("TCP Connection failed with %d\n", error);
    }

    auto* connection = static_cast<tcp::TcpConnection*>(argument);
    if (connection) {
        PLOGD("Marking connection as closed, from on_error.");
        connection->close();
        auto& server = connection->getServer();
        server.onConnectionClosed(connection);
    }
}

err_t on_accept_connection(void* argument,
                           tcp_pcb* new_pcb,
                           err_t err) {
    cyw43_arch_lwip_check();
    PLOGD("Incoming connection detected.");

    if (new_pcb == nullptr) {
        PLOGD("new_pcb is null.");
        return ERR_ARG;
    }

    if (argument == nullptr) {
        PLOGD("arg is null, aborting...");
        tcp_abort(new_pcb);
        return ERR_ABRT;
    }

    if (err != ERR_OK) {
        PLOGD("Error found: %d", err);
        tcp_abort(new_pcb);
        return ERR_ABRT;
    }

    auto* server = static_cast<tcp::Server*>(argument);
    auto* connection = server->createConnection(new_pcb);

    if (!connection) {
        PLOGD("Server has rejected the incoming connection.");
        tcp_abort(new_pcb);
        return ERR_ABRT;
    }

    tcp_arg(new_pcb, connection);

    tcp_recv(new_pcb, on_receive_data);
    tcp_poll(new_pcb, on_poll, kTcpPollInterval);
    tcp_sent(new_pcb, on_sent);
    tcp_err(new_pcb, on_error);

    PLOGD("Incoming connection has been sucessfully accepted.");
    server->onConnected(connection);
    return ERR_OK;
}

} // namespace

namespace tcp {

bool Server::listen(uint16_t port) {
    bool is_listening = _listen_pcb != nullptr;
    if (is_listening) {
        return false;
    }

    cyw43_thread_enter();

    _listen_pcb = create_listen_pcb(port, this);
    if (_listen_pcb != nullptr) {
        PLOGD("Listening connection established.");
        tcp_accept(_listen_pcb, on_accept_connection);
    }

    cyw43_thread_exit();

    return _listen_pcb != nullptr;
}

bool Server::write(uint32_t connection_id, const void* data, uint16_t size) const {
    auto* connection = findConnectionById(connection_id);
    if (connection->write(data, size)) {
        return connection->flush();
    } else {
        return false;
    }
}

void Server::close(uint32_t connection_id) const {
    auto* connection = findConnectionById(connection_id);
    connection->close();
}

TcpConnection* Server::createConnection(tcp_pcb* pcb) {
    if (_connections.size() >= _max_connections) {
        PLOGD("Connection was aborted, as the amount of open connections is at its limit.");
        return nullptr;
    }

    auto connection = std::make_unique<TcpConnection>(*this, pcb);
    auto* connection_ptr = connection.get();
    uint32_t connection_id = connection->id();

    // TODO: check that _connections map is empty.
    _connections[connection_id] = std::move(connection);

    return connection_ptr;
}

void Server::onConnected(TcpConnection* connection) {
    if (!connection) {
        PLOGD("Connected empty connection?!");
        return;
    }

    if (_onConnectedCallback) {
        _onConnectedCallback(connection->id());
    }
}

bool Server::onDataReceived(TcpConnection* connection, uint8_t* data, uint16_t size) {
    if (_onDataReceivedCallback == nullptr) {
        return false;
    }

    if (!connection) {
        PLOGD("Data received for empty connection.");
        return false;
    }

    return _onDataReceivedCallback(connection->id(), data, size);
}

void Server::onConnectionClosed(TcpConnection* connection) {
    if (!connection) {
        PLOGD("Trying to close empty connection.");
        return;
    }

    uint32_t connection_id = connection->id();
    _connections.erase(connection_id);

    if (_onClosedCallback) {
        _onClosedCallback(connection_id);
    }
}

} // namespace tcp
