#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include "ServerConnection.h"
#include "ServerInterface.pb.h"
#include "ServerInfo.pb.h"
#include "TargetMessage.pb.h"
#include "AllMessages.pb.h"
#include "DeliveryResult.pb.h"
#include <string>

class ServerInterface
{
    /**
     * Connection to the server.
     */
    ServerConnection _connection;

    /**
     * This function tries to send request and receive reply from server.
     * Returns true on succes.
     */
    bool _TalkWithServer(const ServerRequest &request, ServerReply &reply);

public:
    /**
     * Obtains server-related information and puts it into output.
     * Returns true on success.
     */
    bool GetSvrInfo(ServerInfo &output);
    /**
     * Tries to send new message to relay server.
     * Returns true on success.
     */
    bool SendMessage(const TargetMessage &message);
    /**
     * Tries to download all user's messages from relay server.
     * Returns true on succes.
     */
    bool GetAllMessages(const std::string &userId, AllMessagesReply &output);
    /**
     * Tries to send messages' delivery result to server.
     * Returns true on success.
     */
    bool SendDeliveryResult(const DeliveryResult &result);
};

#endif // SERVERINTERFACE_H
