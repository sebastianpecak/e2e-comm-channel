#ifndef EMPTYHANDLER_H
#define EMPTYHANDLER_H

#include "CommandLine.h"

/**
 * This is stub handler that does nothing.
 */
class EmptyHandler : public CommandLine::IHandler
{
public:
    virtual void Execute(const Tokenizer &tokens);
};

#endif // EMPTYHANDLER_H
