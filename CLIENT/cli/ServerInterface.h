#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include "ServerConnection.h"
#include "ServerInfo.pb.h"
#include "TargetMessage.pb.h"
#include "ForwardTargetMessage.pb.h"
#include <string>
#include <vector>

class ServerInterface
{
public:
    void SetConnection(ServerConnection *connection);
    bool GetSvrInfo(ServerInfo &output);
    bool SendMessage(const TargetMessage &message);
    bool GetAllMessages(const std::string &userId, std::vector<ForwardTargetMessage> &output);
    bool SendDeliveryResult(const std::string &userId, )
};

#endif // SERVERINTERFACE_H
