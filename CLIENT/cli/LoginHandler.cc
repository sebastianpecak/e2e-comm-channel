#include "LoginHandler.h"
#include <iostream>

LoginHandler::LoginHandler(Session *sesssion, UserKeyStore *keyStore, CommandLine *cli) :
    _session(sesssion),
    _keyStore(keyStore),
    _cli(cli)
{
}

void LoginHandler::Execute(const Tokenizer &tokens)
{
    // Check if there is no user logged in.
    if (_session->IsActive())
    {
        std::cerr << "Need to logout first." << std::endl;
        return;
    }
    // Second token must be user id.
    if (tokens.GetTokensCount() < 2)
    {
        std::cerr << "Missing user id for login command." << std::endl;
        return;
    }
    // Check if user exists.
    if (not _keyStore->IsUserLoggable(tokens.GetToken(1)))
    {
        std::cerr << "This user does not allow login." << std::endl;
        return;
    }
    // Read user password.
    SafeString password;
    std::cout << "Password: "; std::cout.flush();
    _cli->EnablePasswordMode();
    std::cin >> password.str();
    std::cin.ignore();
    _cli->DisablePasswordMode();
    // Try to obtain user's private key from database using given password.
    auto userKey = _keyStore->GetPrivateKey(tokens.GetToken(1), password);
    if (userKey == nullptr)
    {
        std::cerr << std::endl << "Failed to login user. Probably invalid password." << std::endl;
        return;
    }
    // Setup new user session.
    _session->Setup(tokens.GetToken(1), userKey);
    std::cout << std::endl << "User successfully logged in." << std::endl;
}
