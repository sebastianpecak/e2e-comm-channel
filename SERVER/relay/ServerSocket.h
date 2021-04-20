#ifndef SOCKET_H
#define SOCKET_H

#include "ServerInterface.pb.h"
#include "ClassLog.h"
#include "ClientConnection.h"
#include <string>
#include <netinet/in.h>
#include <poll.h>

/**
 * Server's socket that creates listen socket.
 */
class ServerSocket
{
    /**
     * How many connections can be queued before accepting.
     */
    static constexpr int CONNECTIONS_QUEUE_SIZE = 1;
    static constexpr int POLL_SET_SIZE          = 1;

    /**
     * Logger instance.
     */
    ClassLog _log;
    /**
     * Socket's file descriptor.
     */
    int _socket;
    /**
     * Server address and port.
     */
    sockaddr_in _address;
    /**
     * This flag indicates if accept failed due to timeout or not.
     */
    bool _acceptWasTimeout;
    /**
     * File descriptor set for polling socket.
     */
    pollfd _acceptPoll[POLL_SET_SIZE];

public:
    /**
     * Basic ctor.
     */
    ServerSocket();
    /**
     * Basic dtor.
     */
    ~ServerSocket();
    /**
     * Tries to create new socket and bind it with address and port.
     * After that configures connections queue size.
     * Returns true on success.
     */
    bool Open();
    /**
     * Stops listening and closes server's socket (if opened).
     * Returns true on success.
     */
    bool Close();
    /**
     * Waits for new client's connection for ACCEPT_TIMEOUT_MS.
     * Client connection parameters are stored in newClient object.
     * Returns true if new connection was accepted and newClient output is set.
     * Returns false on timeout or socket error.
     * If method retruns false use WasTimeout to check whether it was caused by accept timeout.
     */
    bool TryAccept(ClientConnection &newClient);
    /**
     * Returns true if server socket is opened and valid.
     */
    bool IsOpened() const;
    /**
     * This method returns true if accept failure was caused by timeout (not an error).
     */
    bool WasTimeout() const;
};

#endif // SOCKET_H
