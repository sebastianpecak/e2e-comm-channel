#include "ClientConnectionMITM.h"
#include <cstring>

/**
 * Environment variables names that cofigure MITM operation.
 */
static constexpr char ENV_MITM_ONOFF[]                      = "E2EEM_MITM_ONOFF";
static constexpr char ENV_MITM_MODE[]                       = "E2EEM_MITM_MODE";
static constexpr char ENV_MITM_MODE_VAL_FLIP_RANDOM_BIT[]   = "FRB";
static constexpr char ENV_MITM_MODE_VAL_CHANGE_RECIPIENT[]  = "CR";
static constexpr char ENV_MITM_MODE_VAL_CHANGE_REPLY_TYPE[] = "CRT";
static constexpr char ENV_MITM_MODE_VAL_CHANGE_REPLY_CODE[] = "CRC";
static constexpr int BITS_PER_BYTE                          = 8;

// ClientConnectionMITM::ClientConnectionMITM() :
//     _log("ClientConnectionMITM")
// {
// }

// ClientConnectionMITM::ClientConnectionMITM(ClientConnection *connection) :
//     _log("ClientConnectionMITM"),
//     _connection(connection),
//     _mode(Mode::UNKNOWN)
// {
// }

ClientConnectionMITM::ClientConnectionMITM(ClientConnection &&other) :
    _log("ClientConnectionMITM"),
    _onoff(false),
    _mode(Mode::UNKNOWN)
{
    static_cast<ClientConnection&>(*this) = std::move(other);
    srand(time(nullptr));
    _ReadConfig();
}

void ClientConnectionMITM::_ReadConfig()
{
    // Try to read onoff flag.
    const auto onoff = getenv(ENV_MITM_ONOFF);
    if (onoff != nullptr)
    {
        _onoff = true;
    }
    else
    {
        LOG_INFO() << "There is no E2EEM_MITM_ONOFF variable - MITM simulation off.";
        return;
    }
    // Try to read mode value.
    const auto mode = getenv(ENV_MITM_MODE);
    if (mode != nullptr)
    {
        _mode = _StringToMode(mode);
    }
    else
    {
        LOG_INFO() << "There is no E2EEM_MITM_MODE variable - using default MITM simualtor mode (flip random bit).";
        _mode = Mode::FLIP_RANDOM_BIT;
    }
}

ClientConnectionMITM::Mode ClientConnectionMITM::_StringToMode(const std::string &modeString)
{
    if (modeString == ENV_MITM_MODE_VAL_FLIP_RANDOM_BIT)
    {
        return Mode::FLIP_RANDOM_BIT;
    }
    else
    if (modeString == ENV_MITM_MODE_VAL_CHANGE_RECIPIENT)
    {
        return Mode::CHANGE_RECIPIENT;
    }
    else
    if (modeString == ENV_MITM_MODE_VAL_CHANGE_REPLY_TYPE)
    {
        return Mode::CHANGE_REPLY_TYPE;
    }
    else
    if (modeString == ENV_MITM_MODE_VAL_CHANGE_REPLY_CODE)
    {
        return Mode::CHANGE_REPLY_CODE;
    }
    else
    {
        // Default mode.
        LOG_ERROR() << "Failed to recognize MITM mode " << modeString << ", using default flip random bit.";
        return Mode::FLIP_RANDOM_BIT;
    }
}

bool ClientConnectionMITM::Send(const ServerReply &reply)
{
    // Simulation must be turned on.
    if (not _onoff)
    {
        return ClientConnection::Send(reply);
    }
    // Make a copy of original reply as it is not modifiable.
    auto attackedReply = reply;
    switch (_mode)
    {
    case Mode::FLIP_RANDOM_BIT:
        _FlipRandomBit(attackedReply);
        break;
    case Mode::CHANGE_RECIPIENT:
        _ChangeRecipient(attackedReply);
        break;
    case Mode::CHANGE_REPLY_TYPE:
        _ChangeReplyType(attackedReply);
        break;
    case Mode::CHANGE_REPLY_CODE:
        _ChangeReplyCode(attackedReply);
        break;
    default:
        LOG_ERROR() << "Unknown attack mode " << static_cast<int>(_mode) << '.';
        break;
    }
    // Call base class method.
    return ClientConnection::Send(attackedReply);
}

void ClientConnectionMITM::_FlipRandomBit(ServerReply &reply)
{
    LOG_INFO() << "MITM SIMULATION: Flipping random bit in server response.";
    // It only filps random bit in data field, so it must be preset.
    if (reply.data().size() == 0)
    {
        LOG_INFO() << "Reply has no data field to mess with.";
        return;
    }
    // Select random bit in data field of reply and flip it.
    const auto dataBitsCount = reply.data().size() * BITS_PER_BYTE;
    const auto bitIndex      = rand() % dataBitsCount;
    const auto byteOffset    = bitIndex / 8;
    const auto bitOffset     = bitIndex - (byteOffset * 8);
    uint8_t byteBuffer[1];
    memcpy(byteBuffer, reply.data().data() + byteOffset, 1);
    byteBuffer[0] ^= (1 << bitOffset);
    memset(&reply.mutable_data()->at(0) + byteOffset, byteBuffer[0], 1);
}

void ClientConnectionMITM::_ChangeRecipient(ServerReply &reply)
{
    LOG_ERROR() << "Not implemented yet.";
}

void ClientConnectionMITM::_ChangeReplyType(ServerReply &reply)
{
    LOG_ERROR() << "Not implemented yet.";
}

void ClientConnectionMITM::_ChangeReplyCode(ServerReply &reply)
{
    LOG_ERROR() << "Not implemented yet.";
}
