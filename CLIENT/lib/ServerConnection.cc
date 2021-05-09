#include "ServerConnection.h"
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <iostream>

static constexpr int INVALID_SOCKET    = -1;
static constexpr short SERVER_PORT     = 1111;
static constexpr char SERVER_ADDRESS[] = "127.0.0.1";

ServerConnection::ServerConnection() :
    _log("ServerConnection"),
    _socket(INVALID_SOCKET),
    _svrAddress()
{
    // Make server address.
    _svrAddress.sin_family = AF_INET;
    _svrAddress.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDRESS, &_svrAddress.sin_addr);
}

ServerConnection::~ServerConnection()
{
    static_cast<void>(Close());
}

bool ServerConnection::Open()
{
    // If connection is already opened then return right away.
    if (IsOpened())
    {
        return true;
    }
    // Try to create TCP socket.
    _socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == INVALID_SOCKET)
    {
        LOG_ERROR() << "Failed to open TCP socket, due to: " << errno << '.';
        return false;
    }
    // Try to connect to server.
    if (connect(_socket, reinterpret_cast<sockaddr*>(&_svrAddress), sizeof(_svrAddress)) != 0)
    {
        LOG_ERROR() << "Failed to connect TCP socket, due to: " << errno << '.';
        Close();
        return false;
    }
    // Success.
    return true;
}

bool ServerConnection::Close()
{
    // Socket must be valid for this call.
    if (not IsOpened())
    {
        return true;
    }
    // Try to shutdown and close socket.
    bool result = true;
    if (shutdown(_socket, SHUT_RDWR) != 0)
    {
        LOG_ERROR() << "Failed to shutdown TCP socket, due to: " << errno << '.';
        result = false;
    }
    if (close(_socket) != 0)
    {
        LOG_ERROR() << "Failed to close TCP socket, due to: " << errno << '.';
        result = false;
    }
    // Invaidate socket file descriptor and return result.
    _socket = INVALID_SOCKET;
    return result;
}

bool ServerConnection::IsOpened()
{
    return (_socket != INVALID_SOCKET);
}

bool ServerConnection::Send(const std::string &data)
{
    // Connection must be opened prior to sending data.
    if (not IsOpened())
    {
        LOG_ERROR() << "Opened connection is required for sending.";
        return false;
    }
    // Try to send all data.
    const auto result = send(_socket, data.data(), data.size(), 0);
    if (result < 0)
    {
        LOG_ERROR() << "Failed to send data to server, due to: " << errno << '.';
        return false;
    }
    if (result < data.size())
    {
        LOG_ERROR() << "Sent only " << result << " bytes of data.";
        return false;
    }
    // Success.
    return true;
}

bool ServerConnection::Recv(std::string &buffer)
{
    // Connection must be opened prior to receiving data.
    if (not IsOpened())
    {
        LOG_ERROR() << "Opened connection is required for receiving.";
        return false;
    }
    // Try to receive some data.
    const auto result = recv(_socket, _buffer, sizeof(_buffer), 0);
    if (result < 1)
    {
        LOG_ERROR() << "Failed to receive data from TCP socket, due to: " << errno << '.';
        return false;
    }
    // Copy received data to output buffer.
    buffer.assign(_buffer, result);
    return true;
}
