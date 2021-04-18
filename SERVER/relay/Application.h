#ifndef APPLICATION_H
#define APPLICATION_H

#include "Storage.h"
#include "IRequest.h"
#include "ServerSocket.h"
#include "ClientServant.h"
#include "ClassLog.h"

class Application
{
private:
    /**
     * Class' logger instance.
     */
    ClassLog _log;
    /**
     * Main instance of storage.
     */
    Storage _storage;
    /**
     * Main server socket.
     */
    ServerSocket _socket;
    /**
     * Main instance of client servant class.
     */
    ClientServant _servant;
    /**
     * This flag indicates if server stop was requested.
     */
    bool _stopRequest;

public:
    /**
     * Basic ctor.
     */
    Application();
    /**
     * Runs server processing loop.
     * To stop need to call Stop method.
     */
    void Run();
    /**
     * Request server stop.
     */
    inline void Stop()
    {
        _stopRequest = true;
    }
};

#endif // APPLICATION_H
