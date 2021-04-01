#include "CreateUserHandler.h"
#include <iostream>

CreateUserHandler::CreateUserHandler(UserKeyStore *keyStore, CommandLine *cli) :
    _keyStore(keyStore),
    _cli(cli)
{
}

void CreateUserHandler::Execute(const Tokenizer &tokens)
{
    // Second token must be user id.
    if (tokens.GetTokensCount() < 2)
    {
        std::cerr << "Missing user id for create command." << std::endl;
        return;
    }
    // Given user id cannot be already preset in database.
    if (_keyStore->IsUserKnown(tokens.GetToken(1)))
    {
        std::cerr << "This user already exists." << std::endl;
        return;
    }
    // Read password from user.
    _cli->EnablePasswordMode();
    SafeString pwd, pwdAgain;
    std::cout << "Password: "; std::cout.flush();
    std::cin >> pwd.str();
    std::cout << std::endl << "Again: "; std::cout.flush();
    std::cin >> pwdAgain.str();
    std::cin.ignore();
    _cli->DisablePasswordMode();
    // Check if both pwds are equal.
    if (pwd.str() != pwdAgain.str())
    {
        std::cerr << std::endl << "Passwords are not the same." << std::endl;
        return;
    }
    _keyStore->CreateUser(tokens.GetToken(1), pwd);
    std::cout << std::endl << "New user has been added." << std::endl;
}
