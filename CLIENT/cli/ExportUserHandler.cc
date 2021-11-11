#include "ExportUserHandler.h"

ExportUserHandler::ExportUserHandler(UserKeyStore *keyStore) :
    _keyStore(keyStore)
{
}

void ExportUserHandler::Execute(const Tokenizer &tokens)
{
    // Second token must be user id.
    if (tokens.GetTokensCount() < 2)
    {
        std::cerr << "You must specify target user's id." << std::endl;
        return;
    }
    // User must be in database.
    if (not _keyStore->IsUserKnown(tokens.GetToken(1)))
    {
        std::cerr << "There is no such user." << std::endl;
        return;
    }
    std::cout << tokens.GetToken(1) << "-COMPACT-RECORD: " << _keyStore->ExportCompactRecord(tokens.GetToken(1)) << std::endl;
}
