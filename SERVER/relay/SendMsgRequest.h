#ifndef SENDMSGREQUEST_H
#define SENDMSGREQUEST_H

#include "IRequest.h"
#include <netinet/in.h>
#include "Storage.h"

/**
 * This class handles SEND_MSG request.
 */
class SendMsgRequest : public IRequest
{
private:
    /**
     * Request to be processed.
     */
    ServerRequest _request;
    /**
     * Client/peer info.
     */
    sockaddr_in _peer;
    /**
     * Storage instance.
     */
    Storage *_storage;
    /**
     * This flag indicates if processing of request was successfull.
     */
    bool _isProcessed;

public:
    SendMsgRequest(Storage *storage, const ServerRequest &request, const sockaddr_in &peer);
    virtual void Process();
    virtual ServerReply GetReply() const;
    virtual const sockaddr_in& GetPeer() const;
};

#endif // SENDMSGREQUEST_H
