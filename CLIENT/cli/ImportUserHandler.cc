#include "ImportUserHandler.h"

ImportUserHandler::ImportUserHandler(UserKeyStore *keyStore) :
    _keyStore(keyStore)
{
}

void ImportUserHandler::Execute(const Tokenizer &tokens)
{
    // Second token must be compact-record.
    if (tokens.GetTokensCount() < 2)
    {
        std::cerr << "Missing compact-record value." << std::endl;
        return;
    }
    // Try to import from compact-record.
    if (not _keyStore->ImportCompactRecord(tokens.GetToken(1)))
    {
        std::cerr << "Failed to import user from compact-record." << std::endl;
        return;
    }
}
