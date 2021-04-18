#include "RecvAllMsgRequest.h"

RecvAllMsgRequest::RecvAllMsgRequest(Storage *storage, const ServerRequest &request) :
    _log("RecvAllMsgRequest"),
    _storage(storage),
    _request(request),
    _isProcessed(false)
{
}

void RecvAllMsgRequest::Process()
{
    // Try to parse request message.
    AllMessagesRequest allMsg;
    if (not allMsg.ParseFromString(_request.data()))
    {
        LOG_ERROR() << "Failed to parse AllMessagesRequest.";
        return;
    }
    // Make reply out of undelivered messages.
    for (const auto &msg : _storage->GetAllMessages(allMsg.userid()))
    {
        if (not msg.GetDelivered() && not msg.GetInvalid())
        {
            ForwardTargetMessage *newMsg = _allMsgReply.add_messages();
            newMsg->set_id(msg.GetId());
            newMsg->mutable_msg()->CopyFrom(msg.GetMessage());
        }
    }
    _isProcessed = true;
}

ServerReply RecvAllMsgRequest::GetReply() const
{
    ServerReply reply;
    reply.set_type(_request.type());
    reply.set_reply(_isProcessed ? ServerReplyCode::SUCCESS : ServerReplyCode::FAILURE);
    reply.set_data(_allMsgReply.SerializeAsString());
    return reply;
}
