#include "ServerSocket.h"
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <unistd.h>

static constexpr int INVALID_SOCKET    = -1;
static constexpr short SERVER_PORT     = 1111;
static constexpr char SERVER_ADDRESS[] = "127.0.0.1";

ServerSocket::ServerSocket() :
    _log("ServerSocket"),
    _socket(INVALID_SOCKET),
    _address()
{
    // Prepare address object.
    _address.sin_family = AF_INET;
    _address.sin_port   = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDRESS, &_address.sin_addr);
}

ServerSocket::~ServerSocket()
{
    Close();
}

bool ServerSocket::Open()
{
    // Socket must not be opened yet.
    if (IsOpened())
    {
        LOG_INFO() << "Socket is already opened.";
        return true;
    }
    // Try to create new TCP socket.
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == INVALID_SOCKET)
    {
        LOG_ERROR() << "Failed to create new socket, due to: " << errno << '.';
        return false;
    }
    // Try to bind socket to address and port.
    const auto bindResult = bind(_socket, reinterpret_cast<sockaddr*>(&_address), sizeof(_address));
    if (bindResult != 0)
    {
        LOG_ERROR() << "Failed to bind server socket to address " << SERVER_ADDRESS << ':' << SERVER_PORT << ", due to: " << errno << '.';
        Close();
        return false;
    }
    // Setup waiting connections queue size.
    const auto listenResult = listen(_socket, CONNECTIONS_QUEUE_SIZE);
    if (listenResult != 0)
    {
        LOG_ERROR() << "Failed to setup connections queue size, due to " << errno << '.';
        Close();
        return false;
    }
    // Success.
    return true;
}

bool ServerSocket::Close()
{
    if (IsOpened())
    {
        shutdown(_socket, SHUT_RDWR);
        close(_socket);
        _socket = INVALID_SOCKET;
    }

    return true;
}

bool ServerSocket::Accept(ClientConnection &newClient)
{
    // Try to accept new client's connection.
    const auto newSocket = accept(_socket, nullptr, nullptr);
    if (newSocket < 0)
    {
        LOG_ERROR() << "Failed to accept new client's connection, due to " << errno << '.';
        return false;
    }
    // Create new connection object.
    newClient = ClientConnection(newSocket);
    // Success.
    return true;
}

bool ServerSocket::IsOpened() const
{
    return (_socket != INVALID_SOCKET);
}
