#include "Application.h"

/**
 * This function instantiates Application class
 * and returns it.
 */
static Application& _GetAppInstance();

int main()
{
    _GetAppInstance().Run();
    return 0;
}

Application& _GetAppInstance()
{
    static Session         session;
    static UserKeyStore    keyStore;
    static ServerInterface svrIface;
    static CommandLine     cli(&session);
    static Application     app(&session, &keyStore, &svrIface, &cli);
    return app;
}
