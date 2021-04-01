#ifndef SVRINFOHANDLER_H
#define SVRINFOHANDLER_H

#include "CommandLine.h"
#include "ServerInterface.h"

/**
 * This class handles svr-info command.
 */
class SvrInfoHandler : public CommandLine::IHandler
{
private:
    ServerInterface *_svrIface;

public:
    explicit SvrInfoHandler(ServerInterface *svrIface);
    virtual void Execute(const Tokenizer &tokens);
};

#endif // SVRINFOHANDLER_H
