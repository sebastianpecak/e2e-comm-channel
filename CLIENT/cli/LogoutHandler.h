#ifndef LOGOUTHANDLER_H
#define LOGOUTHANDLER_H

#include "CommandLine.h"
#include "Session.h"

/**
 * This class handles logout command.
 */
class LogoutHandler : public CommandLine::IHandler
{
private:
    Session *_session;

public:
    explicit LogoutHandler(Session *session);
    virtual void Execute(const Tokenizer &tokens);
};

#endif // LOGOUTHANDLER_H
