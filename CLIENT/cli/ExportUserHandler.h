#ifndef EXPORTUSERHANDLER_H
#define EXPORTUSERHANDLER_H

#include "CommandLine.h"
#include "UserKeyStore.h"

/**
 * This class handles export-user command.
 */
class ExportUserHandler : public CommandLine::IHandler
{
private:
    UserKeyStore *_keyStore;

public:
    explicit ExportUserHandler(UserKeyStore *keyStore);
    virtual void Execute(const Tokenizer &tokens);
};

#endif // EXPORTUSERHANDLER_H
