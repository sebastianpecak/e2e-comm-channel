#ifndef SENDHANDLER_H
#define SENDHANDLER_H

#include "CommandLine.h"
#include "Session.h"
#include "UserKeyStore.h"
#include "ServerInterface.h"
#include <cryptopp/osrng.h>

/**
 * This class handles send command.
 */
class SendHandler : public CommandLine::IHandler
{
private:
    Session *_session;
    UserKeyStore *_keyStore;
    ServerInterface *_svrIface;
    CryptoPP::AutoSeededRandomPool _rng;

    /**
     * This function is used to read user's message from standard input.
     * Returns message string.
     */
    std::string _GetUserMessage() const;

public:
    SendHandler(Session *session, UserKeyStore *keyStore, ServerInterface *svrIface);
    virtual void Execute(const Tokenizer &tokens);
};

#endif // SENDHANDLER_H
