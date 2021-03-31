#include "ServerInterface.h"
#include <iostream>

bool ServerInterface::_TalkWithServer(const ServerRequest &request, ServerReply &reply)
{
    // Make sure that connection to server is opened.
    if (not _connection.Open())
    {
        std::cerr << "Failed to open connection to the server." << std::endl;
        return false;
    }
    // Try to request.
    if (not _connection.Send(request.SerializeAsString()))
    {
        std::cerr << "Failed to request server information." << std::endl;
        return false;
    }
    // Try to receive response.
    std::string buffer;
    if (not _connection.Recv(buffer))
    {
        std::cerr << "Failed to receive server information." << std::endl;
        return false;
    }
    // Try to parse server response.
    if (not reply.ParseFromString(buffer))
    {
        std::cerr << "Failed to parse server reply." << std::endl;
        return false;
    }
    // Verify server reply code.
    if (reply.reply() == ServerReplyCode::FAILURE)
    {
        std::cerr << "Server replied with failure." << std::endl;
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
        std::cerr << "Failed to communicate with server." << std::endl;
        return false;
    }
    // Try to parse server info.
    if (not output.ParseFromString(reply.data()))
    {
        std::cerr << "Failed to parse server info." << std::endl;
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
        std::cerr << "Failed to communicate with server." << std::endl;
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
        std::cerr << "Failed to communicate with server." << std::endl;
        return false;
    }
    // Try to parse all messages reply.
    if (not output.ParseFromString(reply.data()))
    {
        std::cerr << "Failed to parse all messages reply." << std::endl;
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
        std::cerr << "Failed to communicate with server." << std::endl;
        return false;
    }
    // Success.
    return true;
}
