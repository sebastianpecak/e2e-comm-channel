#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <memory>
#include <cryptopp/rsa.h>

class Session
{
private:
    /**
     * Currently logged in user's id.
     */
    std::string _userId;
    /**
     * Currently logged in user's private key.
     */
    std::unique_ptr<CryptoPP::RSA::PrivateKey> _userKey;

public:
    ~Session();
    /**
     * Returns true is there is an active user's session.
     */
    bool IsActive() const;
    /**
     * Invalidate current user's session.
     */
    void Invalidate();
    /**
     * Setup new user's session.
     */
    void Setup(const std::string &userId, std::unique_ptr<CryptoPP::RSA::PrivateKey> &userKey);
    /**
     * Returns private key of active session.
     */
    inline const CryptoPP::RSA::PrivateKey& GetKey() const
    {
        return *_userKey;
    }
    /**
     * Returns active user's id.
     */
    inline const std::string& GetId() const
    {
        return _userId;
    }
};

#endif // SESSION_H
