#ifndef GETSVRINFOREQUEST_H
#define GETSVRINFOREQUEST_H

#include "IRequest.h"
#include <netinet/in.h>
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
    /**
     * Reply object.
     */
    //AllMessagesReply _allMsgReply;
    ServerInfo _info;

public:
    GetSvrInfoRequest(Storage *storage, const ServerRequest &request, const sockaddr_in &peer);
    virtual void Process();
    virtual ServerReply GetReply() const;
    virtual const sockaddr_in& GetPeer() const;
};

#endif // GETSVRINFOREQUEST_H
