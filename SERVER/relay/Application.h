#ifndef APPLICATION_H
#define APPLICATION_H

#include "Storage.h"
#include "IRequest.h"

class Application
{
private:
    /**
     * Main application instance of storage.
     */
    Storage *_storage;
    /**
     * Main server UDP socket.
     */
    int _mainSocket;

    void _ProcessRequest(const void *data, unsigned int msgSize, const sockaddr_in &peer, int cliSocket);
    void _SendReply(const IRequest &request, int cliSocket);


public:
    Application(int argc, char **argv);
    int Run();
};

#endif // APPLICATION_H
