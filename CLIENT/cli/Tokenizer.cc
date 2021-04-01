#include "Tokenizer.h"
#include <sstream>

Tokenizer::Tokenizer(const std::string &input, char delimiter)
{
    _Parse(input, delimiter);
}

size_t Tokenizer::GetTokensCount() const
{
    return _tokens.size();
}

std::string Tokenizer::GetToken(size_t index) const
{
    return _tokens[index];
}

void Tokenizer::_Parse(const std::string &input, char delimiter)
{
    std::istringstream inputStream(input);
    std::string token;
    while (not inputStream.eof())
    {
        std::getline(inputStream, token, delimiter);
        if (token.length() > 0)
        {
            _tokens.push_back(token);
        }
    }
}
