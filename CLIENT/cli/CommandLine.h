#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <unordered_map>
#include <string>
#include "Tokenizer.h"
#include "Session.h"

/**
 * This class processes command line input and executes poper command handler.
 */
class CommandLine
{
public:
    /**
     * Command handler interface class declaraction.
     */
    class IHandler;

private:
    /**
     * This map maps command name to its handler object.
     */
    std::unordered_map<std::string, IHandler*> _handlers;
    /**
     * Last command cache.
     */
    std::string _lastCommand;
    /**
     * User's session object.
     */
    Session *_session;

    /**
     * Enable or disbale console echoing.
     */
    void _SetPasswordMode(bool enable) const;

public:
    explicit CommandLine(Session *session);
    ~CommandLine();
    /**
     * Register command handler object.
     * CommandLine instance is responsible for freeing handler memory.
     */
    void RegisterHandler(const std::string &command, IHandler *handler);
    /**
     * This method reads one command and processes it.
     */
    void ProcessCommand();
    /**
     * Returns last command name.
     */
    std::string GetLastCommand() const;
    /**
     * Disable console echoing in password mode.
     */
    void EnablePasswordMode() const;
    /**
     * Enable console echoing.
     */
    void DisablePasswordMode() const;
};

/**
 * Command handler interafce definition.
 */
class CommandLine::IHandler
{
public:
    /**
     * This method receives all command tokens as parameter.
     */
    virtual void Execute(const Tokenizer &tokens) = 0;
};

#endif // COMMANDLINE_H
