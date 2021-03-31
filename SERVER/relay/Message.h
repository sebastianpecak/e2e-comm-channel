#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "TargetMessage.pb.h"

class Message
{
    static constexpr unsigned int ID_SIZE_IN_BYTES = 4;
    /**
     * This is server-internal message id used to identify message accros server.
     */
    std::string _id;
    /**
     * Proper message format defined in INTERFACE library.
     */
    TargetMessage _tgtMsg;
    /**
     * Flag to determine if message has been delivered to target.
     */
    bool _isDelivered;
    bool _isInvalid;

public:
    /**
     * This ctor generates unique message identity.
     */
    explicit Message(const TargetMessage &tgtMsg);
    /**
     * Getters.
     */
    inline const std::string& GetId() const
    {
        return _id;
    }
    inline bool GetDelivered() const
    {
        return _isDelivered;
    }
    inline bool GetInvalid() const
    {
        return _isInvalid;
    }
    inline const TargetMessage& GetMessage() const
    {
        return _tgtMsg;
    }
    /**
     * Setters.
     */
    inline void SetDelivered(bool isDelivered)
    {
        _isDelivered = isDelivered;
    }
    inline void SetInvalid(bool isInvalid)
    {
        _isInvalid = isInvalid;
    }
};

#endif // MESSAGE_H
