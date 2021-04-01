#include "CommandLine.h"
#include <iostream>
#include <unistd.h>
#include <termios.h>

static constexpr char CLI_PROMPT[] = ":$ ";

CommandLine::CommandLine(Session *session) :
    _session(session)
{
}

CommandLine::~CommandLine()
{
    // Free all handlers.
    for (const auto &handler : _handlers)
    {
        delete handler.second;
    }
    _handlers.clear();
    // Enable console echoing.
    DisablePasswordMode();
}

void CommandLine::RegisterHandler(const std::string &command, IHandler *handler)
{
    if (handler != nullptr)
    {
        _handlers[command] = handler;
    }
}

void CommandLine::ProcessCommand()
{
    std::string command;
    printf("%s%s", _session->GetId().c_str(), CLI_PROMPT);
    std::getline(std::cin, command);
    const auto tokens = Tokenizer(command, ' ');
    if (tokens.GetTokensCount() > 0)
    {
        // Try to find command handler.
        const auto found = _handlers.find(tokens.GetToken(0));
        if (found == _handlers.end())
        {
            std::cerr << "Unknown command '" << tokens.GetToken(0) << "'." << std::endl;
            return;
        }
        // Execute command handler and update last command cache.
        _lastCommand = tokens.GetToken(0);
        found->second->Execute(tokens);
    }
}

std::string CommandLine::GetLastCommand() const
{
    return _lastCommand;
}

void CommandLine::_SetPasswordMode(bool enable) const
{
    termios tty = termios();
    static_cast<void>(tcgetattr(STDIN_FILENO, &tty));
    if (enable)
    {
        tty.c_lflag &= ~ECHO;
    }
    else
    {
        tty.c_lflag |= ECHO;
    }
    static_cast<void>(tcsetattr(STDIN_FILENO, TCSANOW, &tty));
}

void CommandLine::EnablePasswordMode() const
{
    _SetPasswordMode(true);
}

void CommandLine::DisablePasswordMode() const
{
    _SetPasswordMode(false);
}
