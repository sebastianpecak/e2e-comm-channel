#include "SendMsgRequest.h"

SendMsgRequest::SendMsgRequest(Storage *storage, const ServerRequest &request) :
    _log("SendMsgRequest"),
    _storage(storage),
    _request(request),
    _isProcessed(false)
{
}

void SendMsgRequest::Process()
{
    // Try to parse request message.
    TargetMessage tgtMsg;
    if (not tgtMsg.ParseFromString(_request.data()))
    {
        LOG_ERROR() << "Failed to parse TargetMessage.";
        return;
    }
    _storage->NewMessage(tgtMsg);
    _isProcessed = true;
}

ServerReply SendMsgRequest::GetReply() const
{
    ServerReply reply;
    reply.set_type(_request.type());
    reply.set_reply(_isProcessed ? ServerReplyCode::SUCCESS : ServerReplyCode::FAILURE);
    return reply;
}
