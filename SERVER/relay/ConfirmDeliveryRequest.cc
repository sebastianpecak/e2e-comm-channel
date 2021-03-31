#include "ConfirmDeliveryRequest.h"
#include "DeliveryResult.pb.h"

ConfirmDeliveryRequest::ConfirmDeliveryRequest(Storage *storage, const ServerRequest &request, const sockaddr_in &peer) :
    _storage(storage),
    _request(request),
    _peer(peer),
    _isProcessed(false)
{
}

void ConfirmDeliveryRequest::Process()
{
    // Try to parse request message.
    DeliveryResult confirmMsg;
    if (not confirmMsg.ParseFromString(_request.data()))
    {
        fprintf(stderr, "ConfirmDeliveryRequest::Process: Failed to parse ConfirmDelivery.");
        return;
    }
    for (const auto &msg : confirmMsg.msgstatus())
    {
        if (msg.second == DeliveryStatus::MSG_OK)
        {
            _storage->ConfirmDelivery(confirmMsg.userid(), msg.first);
        }
        else if (msg.second == DeliveryStatus::MSG_NOK)
        {
            _storage->RemoveMessage(confirmMsg.userid(), msg.first);
        }
        else
        {
            fprintf(stderr, "Unkown delivery message %s status.\n", msg.first.c_str());
        }
    }
    //for (int i = 0; i < confirmMsg.msg; ++i)
    //{
    //    _storage->ConfirmDelivery(confirmMsg.userid(), confirmMsg.messageid(i));
    //}
    _isProcessed = true;
}

ServerReply ConfirmDeliveryRequest::GetReply() const
{
    ServerReply reply;
    reply.set_type(_request.type());
    reply.set_reply(_isProcessed ? ServerReplyCode::SUCCESS : ServerReplyCode::FAILURE);
    return reply;
}

const sockaddr_in& ConfirmDeliveryRequest::GetPeer() const
{
    return _peer;
}
