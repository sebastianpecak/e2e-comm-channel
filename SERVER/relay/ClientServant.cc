#include "ClientServant.h"
#include "SendMsgRequest.h"
#include "RecvAllMsgRequest.h"
#include "ConfirmDeliveryRequest.h"
#include "GetSvrInfoRequest.h"

ClientServant::ClientServant(Storage *storage) :
    _log("ClientServant"),
    _storage(storage)
{
}

ClientServant::~ClientServant()
{
}

void ClientServant::Serve(ClientConnection &&connection)
{
    LOG_INFO() << "Serving new client...";
    // Process all client's requests.
    ServerRequest request;
    while (connection.IsActive())
    {
        // Try to receive client's request.
        if (not connection.Recv(request))
        {
            LOG_INFO() << "No request received.";
            continue;
        }
        // Try to process received request.
        if (not _ProcessRequest(connection, request))
        {
            LOG_ERROR() << "Failed to process client's request.";
            continue;
        }
    }
    LOG_INFO() << "Serving new client ended.";
}

bool ClientServant::_ProcessRequest(ClientConnection &connection, const ServerRequest &request)
{
    // Try to get request handler.
    auto handler = _GetRequestHandler(request.type(), request);
    if (handler == nullptr)
    {
        LOG_ERROR() << "No request handler found for request type " << static_cast<int>(request.type()) << '.';
        return false;
    }
    // Process request and try send response to client.
    handler->Process();
    if (not connection.Send(handler->GetReply()))
    {
        LOG_ERROR() << "Failed to send reply to client.";
        return false;
    }
    // Success.
    return true;
}

std::unique_ptr<IRequest> ClientServant::_GetRequestHandler(ServerRequestType type, const ServerRequest &request) const
{
    switch (type)
    {
    case ServerRequestType::SEND_MSG:
        return std::make_unique<SendMsgRequest>(_storage, request);
    case ServerRequestType::RECV_ALL_MSG:
        return std::make_unique<RecvAllMsgRequest>(_storage, request);
    case ServerRequestType::CONFIRM_DELIVERY:
        return std::make_unique<ConfirmDeliveryRequest>(_storage, request);
    case ServerRequestType::GET_SVR_INFO:
        return std::make_unique<GetSvrInfoRequest>(_storage, request);
    }

    // No such handler.
    return nullptr;
}
