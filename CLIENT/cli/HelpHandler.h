#ifndef HELPHANDLER_H
#define HELPHANDLER_H

#include "CommandLine.h"

/**
 * This class handles help command.
 * It simply displays help to the screen.
 */
class HelpHandler : public CommandLine::IHandler
{
public:
    virtual void Execute(const Tokenizer &tokens);
};

#endif // HELPHANDLER_H
