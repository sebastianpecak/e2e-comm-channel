#include "Application.h"
#include <signal.h>

/**
 * Server application instance.
 */
static Application app;

/**
 * Installs all signal handlers to gracefully end server app.
 */
static void _InstallSignalHandlers();
/**
 * Main signal handler.
 */
static void _SignalHanlder(int signal);

int main(int argc, char **argv)
{
    _InstallSignalHandlers();
    app.Run();
    return 0;
}

void _InstallSignalHandlers()
{
    static_cast<void>(signal(SIGINT , _SignalHanlder));
    static_cast<void>(signal(SIGABRT, _SignalHanlder));
    static_cast<void>(signal(SIGKILL, _SignalHanlder));
}

void _SignalHanlder(int signal)
{
    app.Stop();
}
