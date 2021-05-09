#ifndef APPLICATION_H
#define APPLICATION_H

#include "UserKeyStore.h"
#include "ServerInterface.h"
#include "CommandLine.h"
#include "Session.h"

/**
 * CLI client application class.
 */
class Application
{
private:
    /**
     * Session manager instance.
     */
    Session *_session;
    /**
     * Keys manager instance.
     */
    UserKeyStore *_keyStore;
    /**
     * Simple interface to server which keeps the connection opened all the time.
     */
    ServerInterface *_svrIface;
    /**
     * Command line processor instance.
     */
    CommandLine *_cli;

public:
    /**
     * Basic ctor.
     * It registers all needed command handlers in 'cli' object.
     */
    Application(Session *session, UserKeyStore *keyStore, ServerInterface *svrIface, CommandLine *cli);
    /**
     * This function reads user's commands and processes them properly.
     * It returns to caller when user types 'exit' command.
     */
    void Run();
};

#endif // APPLICATION_H
