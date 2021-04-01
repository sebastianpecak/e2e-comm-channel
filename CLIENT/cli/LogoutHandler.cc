#include "LogoutHandler.h"
#include <iostream>

LogoutHandler::LogoutHandler(Session *session) :
    _session(session)
{
}

void LogoutHandler::Execute(const Tokenizer &tokens)
{
    if (not _session->IsActive())
    {
        std::cout << "No user was logged in." << std::endl;
        return;
    }
    _session->Invalidate();
    std::cout << "User logged out." << std::endl;
}
