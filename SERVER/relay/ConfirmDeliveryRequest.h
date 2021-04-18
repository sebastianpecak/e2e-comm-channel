#ifndef CONFIRMDELIVERYREQUEST_H
#define CONFIRMDELIVERYREQUEST_H

#include "IRequest.h"
#include "Storage.h"
#include "AllMessages.pb.h"
#include "ClassLog.h"

/**
 * This class handles CONFIRM_DELIVERY request.
 */
class ConfirmDeliveryRequest : public IRequest
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
    ConfirmDeliveryRequest(Storage *storage, const ServerRequest &request);
    virtual void Process();
    virtual ServerReply GetReply() const;
};

#endif // CONFIRMDELIVERYREQUEST_H
