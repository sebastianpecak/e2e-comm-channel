#ifndef SENDMSGREQUEST_H
#define SENDMSGREQUEST_H

#include "IRequest.h"
#include <netinet/in.h>
#include "Storage.h"
#include "ClassLog.h"

/**
 * This class handles SEND_MSG request.
 */
class SendMsgRequest : public IRequest
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

public:
    SendMsgRequest(Storage *storage, const ServerRequest &request);
    virtual void Process();
    virtual ServerReply GetReply() const;
};

#endif // SENDMSGREQUEST_H
