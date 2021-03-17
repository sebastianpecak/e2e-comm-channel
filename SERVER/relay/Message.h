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

public:
    /**
     * This ctor generates unique message identity.
     */
    explicit Message(const TargetMessage &tgtMsg);
    /**
     * Getters.
     */
    const std::string& GetId() const;
    bool GetDelivered() const;
    const TargetMessage& GetMessage() const;
    /**
     * Setters.
     */
    void SetDelivered(bool isDelivered);
};

#endif // MESSAGE_H
