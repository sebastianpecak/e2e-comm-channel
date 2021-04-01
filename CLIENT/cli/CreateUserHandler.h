#ifndef CREATEUSERHANDLER_H
#define CREATEUSERHANDLER_H

#include "CommandLine.h"
#include "UserKeyStore.h"

/**
 * This class handles create-user command.
 */
class CreateUserHandler : public CommandLine::IHandler
{
private:
    UserKeyStore *_keyStore;
    CommandLine *_cli;

public:
    CreateUserHandler(UserKeyStore *keyStore, CommandLine *cli);
    virtual void Execute(const Tokenizer &tokens);
};

#endif // CREATEUSERHANDLER_H
