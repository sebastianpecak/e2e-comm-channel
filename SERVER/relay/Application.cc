#include "Application.h"

#include <cstring>
#include <cstdio>
#include <unistd.h>
// INTERFACE library.
#include "ServerInterface.pb.h"
#include "TargetMessage.pb.h"
#include "SourceMessage.pb.h"
// Request processors.
#include "SendMsgRequest.h"
#include "RecvAllMsgRequest.h"
#include "ConfirmDeliveryRequest.h"
#include "GetSvrInfoRequest.h"

#include "ClientConnection.h"

Application::Application() :
    _log("Application"),
    _stopRequest(false),
    _servant(&_storage)
{
}

void Application::Run()
{
    // Try to open server socket and start listening on it.
    if (not _socket.Open())
    {
        LOG_ERROR() << "Failed to prepare server socket.";
        return;
    }
    // New client connection object.
    ClientConnection client;
    // Start accepting new connections from clients.
    while (not _stopRequest)
    {
        // Try to receive new client's request.
        if (not _socket.Accept(client))
        {
            LOG_ERROR() << "Failed to accept new client's connection.";
            continue;
        }

        // Start serving new client.
        _servant.Serve(std::move(client));
    }

    LOG_INFO() << "Exited server's main loop, because stop was requested.";
}
