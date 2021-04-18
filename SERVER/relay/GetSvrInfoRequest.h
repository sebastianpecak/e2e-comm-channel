#ifndef GETSVRINFOREQUEST_H
#define GETSVRINFOREQUEST_H

#include "IRequest.h"
#include "Storage.h"
#include "AllMessages.pb.h"
#include "ServerInfo.pb.h"

/**
 * This class handles GET_SVR_INFO request.
 */
class GetSvrInfoRequest : public IRequest
{
private:
    /**
     * Request to be processed.
     */
    ServerRequest _request;
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
    ServerInfo _info;

public:
    GetSvrInfoRequest(Storage *storage, const ServerRequest &request);
    virtual void Process();
    virtual ServerReply GetReply() const;
};

#endif // GETSVRINFOREQUEST_H
