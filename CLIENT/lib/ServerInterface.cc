#include "ServerInterface.h"
#include <iostream>

ServerInterface::ServerInterface() :
    _log("ServerInterface")
{
}

bool ServerInterface::_TalkWithServer(const ServerRequest &request, ServerReply &reply)
{
    // Make sure that connection to server is opened.
    if (not _connection.Open())
    {
        LOG_ERROR() << "Failed to open connection to the server.";
        return false;
    }
    // Try to request.
    if (not _connection.Send(request.SerializeAsString()))
    {
        LOG_ERROR() << "Failed to request server information.";
        return false;
    }
    // Try to receive response.
    std::string buffer;
    if (not _connection.Recv(buffer))
    {
        LOG_ERROR() << "Failed to receive server information.";
        return false;
    }
    // Try to parse server response.
    if (not reply.ParseFromString(buffer))
    {
        LOG_ERROR() << "Failed to parse server reply.";
        return false;
    }
    // Verify server reply code.
    if (reply.reply() == ServerReplyCode::FAILURE)
    {
        LOG_ERROR() << "Server replied with failure.";
        return false;
    }
    // Success.
    return true;
}

bool ServerInterface::GetSvrInfo(ServerInfo &output)
{
    // Format request frame and prepare reply buffer.
    ServerReply reply;
    ServerRequest request;
    request.set_type(ServerRequestType::GET_SVR_INFO);
    // Try to talk with server.
    if (not _TalkWithServer(request, reply))
    {
        LOG_ERROR() << "Failed to communicate with server.";
        return false;
    }
    // Try to parse server info.
    if (not output.ParseFromString(reply.data()))
    {
        LOG_ERROR() << "Failed to parse server info.";
        return false;
    }
    // Success.
    return true;
}

bool ServerInterface::SendMessage(const TargetMessage &message)
{
    // Format request frame and prepare reply buffer.
    ServerReply reply;
    ServerRequest request;
    request.set_type(ServerRequestType::SEND_MSG);
    request.set_data(message.SerializeAsString());
    // Try to talk with server.
    if (not _TalkWithServer(request, reply))
    {
        LOG_ERROR() << "Failed to communicate with server.";
        return false;
    }
    // Success.
    return true;
}

bool ServerInterface::GetAllMessages(const std::string &userId, AllMessagesReply &output)
{
    // Format request frame and prepare reply buffer.
    ServerReply reply;
    ServerRequest request;
    AllMessagesRequest allMsgReq;
    allMsgReq.set_userid(userId);
    request.set_type(ServerRequestType::RECV_ALL_MSG);
    request.set_data(allMsgReq.SerializeAsString());
    // Try to talk with server.
    if (not _TalkWithServer(request, reply))
    {
        LOG_ERROR() << "Failed to communicate with server.";
        return false;
    }
    // Try to parse all messages reply.
    if (not output.ParseFromString(reply.data()))
    {
        LOG_ERROR() << "Failed to parse all messages reply.";
        return false;
    }
    // Success.
    return true;
}

bool ServerInterface::SendDeliveryResult(const DeliveryResult &result)
{
    // Format request frame and prepare reply buffer.
    ServerReply reply;
    ServerRequest request;
    request.set_type(ServerRequestType::CONFIRM_DELIVERY);
    request.set_data(result.SerializeAsString());
    // Try to talk with server.
    if (not _TalkWithServer(request, reply))
    {
        LOG_ERROR() << "Failed to communicate with server.";
        return false;
    }
    // Success.
    return true;
}
