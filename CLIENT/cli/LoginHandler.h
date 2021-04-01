#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include "CommandLine.h"
#include "Session.h"
#include "UserKeyStore.h"

/**
 * This class handles login command.
 */
class LoginHandler : public CommandLine::IHandler
{
private:
    Session *_session;
    UserKeyStore *_keyStore;
    CommandLine *_cli;

public:
    LoginHandler(Session *sesssion, UserKeyStore *keyStore, CommandLine *cli);
    virtual void Execute(const Tokenizer &tokens);
};

#endif // LOGINHANDLER_H
