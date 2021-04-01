#ifndef RECVHANDLER_H
#define RECVHANDLER_H

#include "CommandLine.h"
#include "Session.h"
#include "UserKeyStore.h"
#include "ServerInterface.h"
#include <cryptopp/osrng.h>

/**
 * This class handles recv command.
 */
class RecvHandler : public CommandLine::IHandler
{
private:
    Session *_session;
    UserKeyStore *_keyStore;
    ServerInterface *_svrIface;
    CryptoPP::AutoSeededRandomPool _rng;

public:
    RecvHandler(Session *session, UserKeyStore *keyStore, ServerInterface *svrIface);
    virtual void Execute(const Tokenizer &tokens);
};

#endif // RECVHANDLER_H
