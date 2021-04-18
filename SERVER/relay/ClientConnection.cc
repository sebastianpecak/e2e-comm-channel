#include "ClientConnection.h"
#include <sys/socket.h>
#include <unistd.h>

static constexpr int INVALID_SOCKET = -1;

ClientConnection::ClientConnection() :
    ClientConnection(INVALID_SOCKET)
{
}

ClientConnection::ClientConnection(int socket) :
    _log("ClientConnection"),
    _socket(socket),
    _endOfFile(false)
{
}

ClientConnection::~ClientConnection()
{
    Close();
}

void ClientConnection::operator=(ClientConnection &&other)
{
    // Close currently opened socket and move other to this.
    Close();
    _socket       = other._socket;
    other._socket = INVALID_SOCKET;
}

bool ClientConnection::Send(const ServerReply &reply)
{
    // Connection must be active.
    if (not IsActive())
    {
        LOG_ERROR() << "Connection is inactive.";
        return false;
    }
    // Serialize reply.
    const auto serializedReply = reply.SerializeAsString();
    // Try to send reply back to client.
    const auto result = send(_socket, serializedReply.data(), serializedReply.size(), 0);
    if (result < serializedReply.size())
    {
        LOG_ERROR() << "Failed to send reply to client, due to " << errno << ". Sent only " << result << " bytes out of " << serializedReply.size() << '.';
        return false;
    }
    // Success.
    return true;
}

bool ClientConnection::Recv(ServerRequest &request)
{
    // Connection must be active.
    if (not IsActive())
    {
        LOG_ERROR() << "Connection is inactive.";
        return false;
    }
    // Try to receive 
    const auto result = recv(_socket, _buffer, sizeof(_buffer), 0);
    if (result < 0)
    {
        LOG_ERROR() << "Failed to receive data from peer, due to " << errno << '.';
        return false;
    }
    else if (result == 0)
    {
        LOG_INFO() << "Detected end-of-file on connection.";
        _endOfFile = true;
        return true;
    }
    // Try to deserialize message.
    if (not request.ParseFromArray(_buffer, result))
    {
        LOG_ERROR() << "Failed to parse client's request.";
        return false;
    }
    // Success.
    return true;
}

bool ClientConnection::Close()
{
    if (IsActive())
    {
        shutdown(_socket, SHUT_RDWR);
        close(_socket);
        _socket = INVALID_SOCKET;
    }

    return true;
}

bool ClientConnection::IsActive() const
{
    return (not _endOfFile && _socket != INVALID_SOCKET);
}
