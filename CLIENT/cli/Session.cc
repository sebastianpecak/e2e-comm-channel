#include "Session.h"

Session::~Session()
{
    if (IsActive())
    {
        Invalidate();
    }
}

bool Session::IsActive() const
{
    return (_userId.length() > 0 && _userKey != nullptr);
}

void Session::Invalidate()
{
    _userId.clear();
    _userKey.reset();
}

void Session::Setup(const std::string &userId, std::unique_ptr<CryptoPP::RSA::PrivateKey> &userKey)
{
    _userId  = userId;
    _userKey = std::move(userKey);
}
