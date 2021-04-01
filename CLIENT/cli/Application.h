#ifndef APPLICATION_H
#define APPLICATION_H

#include "UserKeyStore.h"
#include "ServerInterface.h"
#include "CommandLine.h"
#include "Session.h"

class Application
{
private:
    Session _session;
    UserKeyStore _keyStore;
    ServerInterface _svrIface;
    CommandLine _cli;

public:
    Application();
    void Run();
};

#endif // APPLICATION_H
