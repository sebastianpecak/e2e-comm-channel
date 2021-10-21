#ifndef CLIENTCONNECTIONMITM_H
#define CLIENTCONNECTIONMITM_H

#include "ClientConnection.h"
#include "ClassLog.h"

/**
 * This class is a wrapper for a ClientConnection object
 * and ads man-in-the-middle attack-simulation layer.
 * It is a implementation of Decorator desing pattern.
 */
class ClientConnectionMITM : public ClientConnection
{
private:
    /**
     * Mode of operation of man-in-the-middle attack.
     */
    enum class Mode
    {
        UNKNOWN,
        FLIP_RANDOM_BIT,
        CHANGE_RECIPIENT,
        CHANGE_REPLY_TYPE,
        CHANGE_REPLY_CODE,
        OUT_OF_BOUND
    };
    /**
     * Logger instance.
     */
    ClassLog _log;
    /**
     * Pointer to object that is being decorated.
     */
    //ClientConnection *_connection;
    /**
     * Cached MITM config.
     */
    bool _onoff;
    Mode _mode;
    /**
     * Read config from environment.
     */
    void _ReadConfig();
    /**
     * This function converts mode string read from environment to Mode enum value.
     */
    Mode _StringToMode(const std::string &modeString);
    /**
     * Set of methods that mess with user's message.
     * _FlipRandomBit: changes random bit in data filed of ServerReply object.
     */
    void _FlipRandomBit(ServerReply &reply);
    void _ChangeRecipient(ServerReply &reply);
    void _ChangeReplyType(ServerReply &reply);
    void _ChangeReplyCode(ServerReply &reply);

public:
    //ClientConnectionMITM();
    /**
     * Construct decorator object with other object of base class.
     * This instance is not responsible for freeing 'connection' related resources.
     */
   // explicit ClientConnectionMITM(ClientConnection *connection);//:
    //     _connection(connection)
    // {
    // }
    ClientConnectionMITM(ClientConnection &&other);
    /**
     * This function will simulate MITM attack on path
     * from server to client.
     */
    bool Send(const ServerReply &reply) override;
};

#endif // CLIENTCONNECTIONMITM_H
