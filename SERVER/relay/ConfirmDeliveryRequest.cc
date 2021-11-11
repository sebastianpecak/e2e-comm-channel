#include "ConfirmDeliveryRequest.h"
#include "DeliveryResult.pb.h"

ConfirmDeliveryRequest::ConfirmDeliveryRequest(Storage *storage, const ServerRequest &request) :
    _log("ConfirmDeliveryRequest"),
    _storage(storage),
    _request(request),
    _isProcessed(false)
{
}

void ConfirmDeliveryRequest::Process()
{
    // Try to parse request message.
    DeliveryResult confirmMsg;
    if (not confirmMsg.ParseFromString(_request.data()))
    {
        LOG_ERROR() << "Failed to parse ConfirmDelivery.";
        return;
    }
    for (const auto &msg : confirmMsg.msgstatus())
    {
        if (msg.second == DeliveryStatus::MSG_OK)
        {
            _storage->ConfirmDelivery(confirmMsg.userid(), msg.first);
        }
        // NOK can be result of communication channel being attacked.
        // Need to introduce some kind of send-retry counter instead of removing message right away.
        else if (msg.second == DeliveryStatus::MSG_NOK)
        {
            //_storage->RemoveMessage(confirmMsg.userid(), msg.first);
        }
        else
        {
            //LOG_ERROR() << "Unkown delivery message status " << msg.first << '.';
        }
    }
    _isProcessed = true;
}

ServerReply ConfirmDeliveryRequest::GetReply() const
{
    ServerReply reply;
    reply.set_type(_request.type());
    reply.set_reply(_isProcessed ? ServerReplyCode::SUCCESS : ServerReplyCode::FAILURE);
    return reply;
}
