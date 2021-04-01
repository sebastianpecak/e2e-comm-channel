#ifndef IMPORTUSERHANDLER_H
#define IMPORTUSERHANDLER_H

#include "CommandLine.h"
#include "UserKeyStore.h"

/**
 * This class handles import-user command.
 */
class ImportUserHandler : public CommandLine::IHandler
{
private:
    UserKeyStore *_keyStore;

public:
    explicit ImportUserHandler(UserKeyStore *keyStore);
    virtual void Execute(const Tokenizer &tokens);
};

#endif // IMPORTUSERHANDLER_H
