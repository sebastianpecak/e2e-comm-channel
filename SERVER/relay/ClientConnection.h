#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include "Noncopyable.h"
#include "ServerInterface.pb.h"
#include "ClassLog.h"
#include <poll.h>

/**
 * Create macro which sets methods as virtual if needed.
 */
#ifdef MITM_SIMULATION
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif // MITM_SIMULATION

/**
 * This is movable, but not copyable class that represents client's socket.
 * Using this class we and send to and receive from client.
 */
class ClientConnection : public Noncopyable
{
private:
    /**
     * Size of reception buffer.
     */
    static constexpr int RECEPTION_BUFFER_SIZE = 1 << 12;
    static constexpr int POLL_SET_SIZE         = 1;

    /**
     * Logger instance.
     */
    ClassLog _log;
    /**
     * Socket where client is connected to.
     */
    int _socket;
    /**
     * Reception buffer.
     */
    unsigned char _buffer[RECEPTION_BUFFER_SIZE];
    /**
     * This flag indicates if there was end-of-file in stream.
     */
    bool _endOfFile;
    /**
     * This flag indicates some error condition or program interuption.
     */
    bool _forceStop;
    /**
     * File descriptor set for polling socket.
     */
    pollfd _recvPoll[POLL_SET_SIZE];

    /**
     * This is internal function.
     * Returns true if socket is opened and cleanup is needed.
     */
    bool _IsOpened() const;

public:
    /**
     * This ctor create invalid connection object.
     */
    ClientConnection();
    /**
     * Create connection object based on file descriptor.
     */
    explicit ClientConnection(int socket);
    /**
     * Basic dtor. Takes care to free resources.
     */
    ~ClientConnection();
    /**
     * Only moving is supported
     */
    void operator=(ClientConnection &&other);
    /**
     * Sends request to peer.
     * Returns true on success.
     */
    VIRTUAL bool Send(const ServerReply &reply);
    /**
     * Waits for request from peer.
     * Returns true if there was a request received.
     */
    VIRTUAL bool Recv(ServerRequest &request);
    /**
     * Invalidate and close connection.
     * Returns true on success.
     */
    bool Close();
    /**
     * Returns true if there is an opened and active connection.
     */
    bool IsActive() const;
};

#endif // CLIENTCONNECTION_H
