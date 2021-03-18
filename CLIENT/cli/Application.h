#ifndef APPLICATION_H
#define APPLICATION_H

#include "System.h"
#include "Configuration.h"
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <unordered_map>

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
    /**
     * This is temporary store which maps user name to RSA key.
     */
    std::unordered_map<std::string, CryptoPP::RSA::PrivateKey> _keys;
    CryptoPP::AutoSeededRandomPool rng;

    void _Initialize();
    void _ProcessCommand(const std::string &command);
    void _DisplayHelp() const;
    void _Send(const std::string &command);
    void _Recv(const std::string &command);
    void _SvrInfo(const std::string &command) const;
    void _Exit(const std::string &command);
    void _GenerateKeyIfNeeded(const std::string &userId);
    CryptoPP::RSA::PrivateKey _GetPrivateKey(const std::string &userId) const;
    CryptoPP::RSA::PublicKey _GetPublicKey(const std::string &userId) const;
public:
    Application(int argc, char **argv);
    int Run();
};

#endif // APPLICATION_H
