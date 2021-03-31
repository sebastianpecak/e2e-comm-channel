#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <netinet/in.h>
#include <string>

class ServerConnection
{
private:
    /**
     * Reception buffer size.
     */
    static constexpr size_t BUFFER_SIZE = 4096;
    /**
     * Socket file descriptor.
     */
    int _socket;
    /**
     * This object contains server address.
     */
    sockaddr_in _svrAddress;
    /**
     * Reception buffer.
     */
    char _buffer[BUFFER_SIZE];

public:
    ServerConnection();
    /**
     * Tries to open stream connection to application server.
     * Returns true on success.
     */
    bool Open();
    /**
     * Shuts down existing TCP connection and closes socket.
     * If connection is not established then it return false.
     */
    bool Close();
    /**
     * Tries to send data to server.
     * Returns true if all bytes were successfully sent.
     */
    bool Send(const std::string &data);
    /**
     * Tries to receive data from server and put it into buffer.
     * Returns true if any data were receieved.
     */
    bool Recv(std::string &buffer);
    /**
     * Checks if connection to server is established.
     * Returns true if it is.
     */
    bool IsOpened();
};

#endif // SERVERCONNECTION_H
