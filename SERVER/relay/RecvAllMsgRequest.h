#ifndef RECVALLMSGREQUEST_H
#define RECVALLMSGREQUEST_H

#include "IRequest.h"
#include "Storage.h"
#include "AllMessages.pb.h"
#include "ClassLog.h"

/**
 * This class handles RECV_ALL_MSG request.
 */
class RecvAllMsgRequest : public IRequest
{
private:
    /**
     * Logger instance.
     */
    ClassLog _log;
    /**
     * Request to be processed.
     */
    const ServerRequest &_request;
    /**
     * Storage instance.
     */
    Storage *_storage;
    /**
     * This flag indicates if processing of request was successfull.
     */
    bool _isProcessed;
    /**
     * Reply object.
     */
    AllMessagesReply _allMsgReply;

public:
    RecvAllMsgRequest(Storage *storage, const ServerRequest &request);
    virtual void Process();
    virtual ServerReply GetReply() const;
};

#endif // RECVALLMSGREQUEST_H
