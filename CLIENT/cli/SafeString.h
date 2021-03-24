#ifndef SAFESTRING_H
#define SAFESTRING_H

#include <string>
#include <cstring>

/**
 * This is wrapper class for std::string which zeroes used memory at dtor.
 * Needed when storing passwords/keys in string object.
 */
class SafeString
{
private:
    std::string _string;

public:
    ~SafeString()
    {
        memset(_string.begin().base(), 0, _string.size());
    }
    inline std::string& str()
    {
        return _string;
    }
    inline const std::string& str() const
    {
        return _string;
    }
};

#endif // SAFESTRING_H
