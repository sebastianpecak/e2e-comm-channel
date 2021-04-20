#include "ClientConnection.h"
#include <sys/socket.h>
#include <unistd.h>

static constexpr int INVALID_SOCKET  = -1;
static constexpr int RECV_TIMEOUT_MS = 1000;

ClientConnection::ClientConnection() :
    ClientConnection(INVALID_SOCKET)
{
}

ClientConnection::ClientConnection(int socket) :
    _log("ClientConnection"),
    _socket(socket),
    _endOfFile(false),
    _forceStop(false),
    _recvPoll()
{
    _recvPoll[0].fd     = socket;
    _recvPoll[0].events = POLLIN;
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
    memcpy(_recvPoll, other._recvPoll, sizeof(_recvPoll));
    memset(other._recvPoll, 0, sizeof(other._recvPoll));
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
    // Try to receive for RECV_TIMEOUT_MS.
    const auto pollResult = poll(_recvPoll, POLL_SET_SIZE, RECV_TIMEOUT_MS);
    if (pollResult < 0)
    {
        LOG_ERROR() << "Failed to poll socket for reading, due to " << errno << '.';
        _forceStop = true;
        return false;
    }
    else if (pollResult == 0)
    {
        LOG_INFO() << "Receive timeout has occured.";
        return false;
    }
    // Try to receive.
    const auto result = recv(_socket, _buffer, sizeof(_buffer), 0);
    if (result < 0)
    {
        LOG_ERROR() << "Failed to receive data from peer, due to " << errno << '.';
        _forceStop = true;
        return false;
    }
    else if (result == 0)
    {
        LOG_INFO() << "Detected end-of-file on connection.";
        _endOfFile = true;
        return false;
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
    if (_IsOpened())
    {
        shutdown(_socket, SHUT_RDWR);
        close(_socket);
        _socket = INVALID_SOCKET;
    }

    return true;
}

bool ClientConnection::IsActive() const
{
    return (_IsOpened() && not _endOfFile && not _forceStop);
}

bool ClientConnection::_IsOpened() const
{
    return (_socket != INVALID_SOCKET);
}
