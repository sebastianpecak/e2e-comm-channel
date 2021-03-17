#ifndef APPLICATION_H
#define APPLICATION_H

#include "System.h"
#include "Configuration.h"

class Application
{
private:
    System _system;
    /**
     * Application configuration object.
     * Could be turned into pointer/reference to enable dependency injection.
     */
    Configuration _config;
    /**
     * Flag that says when to stop execution and exit gracefully.
     */
    bool _needToStop;
    /**
     * User name read from console.
     */
    std::string _userName;
    /**
     * Main application socket.
     */
    //int _mainSocket;

    void _Initialize();
    void _ProcessCommand(const std::string &command);
    void _DisplayHelp() const;
    void _Send(const std::string &command) const;
    void _Recv(const std::string &command) const;
    void _SvrInfo(const std::string &command) const;
    void _Exit(const std::string &command);
public:
    Application(int argc, char **argv);
    int Run();
};

#endif // APPLICATION_H
