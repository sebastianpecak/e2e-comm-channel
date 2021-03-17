#include "ConfirmDeliveryRequest.h"
#include "ConfirmDelivery.pb.h"

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
    ConfirmDelivery confirmMsg;
    if (not confirmMsg.ParseFromString(_request.data()))
    {
        fprintf(stderr, "ConfirmDeliveryRequest::Process: Failed to parse ConfirmDelivery.");
        return;
    }
    for (int i = 0; i < confirmMsg.messageid_size(); ++i)
    {
        _storage->ConfirmDelivery(confirmMsg.userid(), confirmMsg.messageid(i));
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

const sockaddr_in& ConfirmDeliveryRequest::GetPeer() const
{
    return _peer;
}
