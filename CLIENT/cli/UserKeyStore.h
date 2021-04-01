#ifndef USERKEYSTORE_H
#define USERKEYSTORE_H

#include "AppDb.pb.h"
#include <cryptopp/rsa.h>
#include <memory>
#include "SafeString.h"

/**
 * This class handles storing user key.
 */
class UserKeyStore
{
public:
    AppDb _db;
    /**
     * Decrypts and returns private key.
     */
    std::unique_ptr<CryptoPP::RSA::PrivateKey> _DecryptKey(const std::string &encKeyBytes, const SafeString &pwd);
    std::string _dbPath;

public:
    UserKeyStore();
    ~UserKeyStore();
    /**
     * Returns true if user with given id exists in database and has private key set.
     * Entries without private key are just known recipients (their public key).
     */
    bool IsUserLoggable(const std::string &userId);
    /**
     * Returns true if database contains record for given userId.
     * Private key is not required.
     */
    bool IsUserKnown(const std::string &userId);
    /**
     * Precondition: user must exist - see method above.
     * Tries to decrypt and deserialize private key stored in database.
     * Returns nullptr on failure.
     * Memory pointed by return value must be freed by caller.
     */
    std::unique_ptr<CryptoPP::RSA::PrivateKey> GetPrivateKey(const std::string &userId, const SafeString &pwd);
    /**
     * This function creates new user entry in storage.
     * It generates new RSA key and encrypts it with given password.
     */
    void CreateUser(const std::string &userId, const SafeString &pwd);
    /**
     * Precondition: user must exist - check using IsUserKnown.
     * Returns user's public key.
     */
    CryptoPP::RSA::PublicKey GetPublicKey(const std::string &userId);
    /**
     * Precondition: user must be known - use IsUserKnown.
     * This method is used to export user's public key.
     * Output is string in compact-record fromat.
     */
    std::string ExportCompactRecord(const std::string &userId);
    /**
     * This function imports user's public key from compact-record.
     * Returns true on success.
     */
    bool ImportCompactRecord(const std::string &compactRecord);
};

#endif // USERKEYSTORE_H
