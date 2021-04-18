#ifndef SOCKET_H
#define SOCKET_H

#include "ServerInterface.pb.h"
#include "ClassLog.h"
#include "ClientConnection.h"
#include <string>
#include <netinet/in.h>

/**
 * Server's socket that creates listen socket.
 */
class ServerSocket
{
    /**
     * How many connections can be queued before accepting.
     */
    static constexpr int CONNECTIONS_QUEUE_SIZE = 1;

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
     * Waits for new client's connection.
     * Client connection parameters are stored in newClient object.
     * Returns true on success.
     */
    bool Accept(ClientConnection &newClient);
    /**
     * Returns true if server socket is opened and valid.
     */
    bool IsOpened() const;
};

#endif // SOCKET_H
