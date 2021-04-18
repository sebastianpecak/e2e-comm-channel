#ifndef IREQUEST_H
#define IREQUEST_H

#include "ServerInterface.pb.h"

/**
 * This is interface class which is implemented by specific request processor.
 * Implementation of strategy design pattern.
 */
class IRequest
{
public:
    virtual ~IRequest() {}
    /**
     * Do request prcessing.
     * Returns reply to be sent to the client.
     */
    virtual void Process() = 0;
    /**
     * Creates and returns server reply message based on processing result.
     */
    virtual ServerReply GetReply() const = 0;
};

#endif // IREQUEST_H
