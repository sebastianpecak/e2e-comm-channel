#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

/**
 * This class is used to handle multi-parameter command line input.
 * It parses input string into individual tokens delimited by given delimiter.
 */
class Tokenizer
{
    /**
     * Tokens buffer.
     */
    std::vector<std::string> _tokens;

    /**
     * Internal function used to tokenize input.
     */
    void _Parse(const std::string &input, char delimiter);

public:
    /**
     * Construct new instance with given input string and try to tokenize it using given delimiter.
     */
    Tokenizer(const std::string &input, char delimiter);
    /**
     * Returns number of tokens from parsed input.
     */
    size_t GetTokensCount() const;
    /**
     * Returns token by index.
     */
    std::string GetToken(size_t index) const;
};

#endif // TOKENIZER_H
