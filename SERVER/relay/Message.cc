#include "Message.h"
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>

Message::Message(const TargetMessage &tgtMsg) :
    _tgtMsg(tgtMsg)
{
    using namespace CryptoPP;
    // Generate random id.
    AutoSeededRandomPool rng;
    byte randomId[ID_SIZE_IN_BYTES];
    rng.GenerateBlock(randomId, sizeof(randomId));
    // Convert to string.
    StringSource(
        randomId,
        sizeof(randomId),
        true,
        new HexEncoder(
            new StringSink(_id)
        )
    );
}

const std::string& Message::GetId() const
{
    return _id;
}

bool Message::GetDelivered() const
{
    return _isDelivered;
}

const TargetMessage& Message::GetMessage() const
{
    return _tgtMsg;
}

void Message::SetDelivered(bool isDelivered)
{
    _isDelivered = isDelivered;
}
