#ifndef SOCKET_H
#define SOCKET_H

#include <string>

/**
 * Socket class wrapper for server.
 * It implements UDP socket.
 */
class Socket
{
public:
    bool Open();
    bool Close();
    bool Bind(const std::string &address, short port);
    bool Send(const void *data, unsigned int dataSize, const std::string &address, short port);
    bool Recv(void *buffer, unsigned int bufferSize);
};

#endif // SOCKET_H
