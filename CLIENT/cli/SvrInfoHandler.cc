#include "SvrInfoHandler.h"
#include "ServerInfo.pb.h"
#include <iostream>

SvrInfoHandler::SvrInfoHandler(ServerInterface *svrIface) :
    _svrIface(svrIface)
{
}

void SvrInfoHandler::Execute(const Tokenizer&)
{
    ServerInfo svrInfo;
    if (not _svrIface->GetSvrInfo(svrInfo))
    {
        std::cerr << "Failed to get server info." << std::endl;
        return;
    }
    std::cout << "SERVER NAME: " << svrInfo.name() << ", SERVER VERSION: " << svrInfo.version() << std::endl;
}
