#include "UserKeyStore.h"
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/aes.h>
#include <cryptopp/default.h>
#include <cryptopp/filters.h>
#include <fstream>

#define DB_FILE_NAME "cli.storage"

UserKeyStore::UserKeyStore()
{
    // Load database if exists.
    std::ifstream dbFile(DB_FILE_NAME, std::ios::in | std::ios::binary);
    if (not dbFile.is_open())
    {
        std::cout << "Database file does not exist." << std::endl;
        return;
    }
    // Deserialize file.
    if (not _db.ParseFromIstream(&dbFile))
    {
        std::cerr << "Failed to parse database file." << std::endl;
        return;
    }
}

UserKeyStore::~UserKeyStore()
{
    // Dump database to file.
    _db.SerializeToOstream(new std::ofstream(DB_FILE_NAME, std::ios::out | std::ios::binary));
}

bool UserKeyStore::IsUserLoggable(const std::string &userId)
{
    // Find user in loaded database by id.
    for (int i = 0; i < _db.records_size(); ++i)
    {
        // If we found user.
        if (_db.records(i).userid() == userId)
        {
            return _db.records(i).privatekey().size() > 0;
        }
    }

    return false;
}

bool UserKeyStore::IsUserKnown(const std::string &userId)
{
    // Find user in loaded database by id.
    for (int i = 0; i < _db.records_size(); ++i)
    {
        // If we found user.
        if (_db.records(i).userid() == userId)
        {
            return true;
        }
    }

    return false;
}

std::unique_ptr<CryptoPP::RSA::PrivateKey> UserKeyStore::GetPrivateKey(const std::string &userId, const std::string &pwd)
{
    // Find user in loaded database by id.
    for (int i = 0; i < _db.records_size(); ++i)
    {
        // If we found user.
        if (_db.records(i).userid() == userId)
        {
            return _DecryptKey(_db.records(i).privatekey(), pwd);
        }
    }

    return nullptr;
}

std::unique_ptr<CryptoPP::RSA::PrivateKey> UserKeyStore::_DecryptKey(const std::string &encKeyBytes, const std::string &pwd)
{
    using namespace CryptoPP;

    // Make AES key out of password.
    SecByteBlock aesKey(AES::MAX_KEYLENGTH);
    SHA256 pwdHasher;
    pwdHasher.CalculateDigest(aesKey, reinterpret_cast<const byte*>(pwd.data()), pwd.size());
    // Decrypt private key.
    std::string decPrivKey;
    //ByteQueue decPrivKey;
    try
    {
        StringSource(
            reinterpret_cast<const byte*>(encKeyBytes.data()),
            encKeyBytes.size(),
            true,
            new DefaultDecryptorWithMAC(
                (byte*)aesKey.data(),
                aesKey.size(),
                new StringSink(decPrivKey)
            )
        );
    }
    catch (const std::exception &e)
    {
        return nullptr;
    }
    // Make private key.
    StringSource decPrivKeySrc(decPrivKey, true);
    std::unique_ptr<RSA::PrivateKey> key = std::make_unique<RSA::PrivateKey>();
    key->Load(decPrivKeySrc);
    return key;
}

void UserKeyStore::CreateUser(const std::string &userId, const std::string &pwd)
{
    using namespace CryptoPP;

    AppDbRecord *newRecord = _db.add_records();
    newRecord->set_userid(userId);
    // Generate new RSA key.
    AutoSeededRandomPool rng;
    RSA::PrivateKey newKey;
    newKey.GenerateRandomWithKeySize(rng, 2048);
    RSA::PublicKey newKeyPub(newKey);
    // Serialize public key and save in db.
    StringSink serializedPubKeySink(*newRecord->mutable_publickey());
    newKeyPub.Save(serializedPubKeySink);
    // Serialize private key and encrypt it with hashed password.
    std::string serializedPrivKey;
    StringSink serializedPrivKeySink(serializedPrivKey);
    newKey.Save(serializedPrivKeySink);
    SecByteBlock aesKey(AES::MAX_KEYLENGTH);
    SHA256 hasher;
    hasher.CalculateDigest(aesKey, reinterpret_cast<const byte*>(pwd.data()), pwd.size());
    StringSource(
        reinterpret_cast<const byte*>(serializedPrivKey.data()),
        serializedPrivKey.size(),
        true,
        new DefaultEncryptorWithMAC(
            reinterpret_cast<const byte*>(aesKey.data()),
            aesKey.size(),
            new StringSink(*newRecord->mutable_privatekey())
        )
    );
}

CryptoPP::RSA::PublicKey UserKeyStore::GetPublicKey(const std::string &userId)
{
    // Find user in loaded database by id.
    for (int i = 0; i < _db.records_size(); ++i)
    {
        // If we found user.
        if (_db.records(i).userid() == userId)
        {
            // Deserialize and return public key.
            CryptoPP::RSA::PublicKey key;
            CryptoPP::StringSource keySource(_db.records(i).publickey(), true);
            key.Load(keySource);
            return key;
        }
    }

    // Should never happen.
    // Call IsUserKnown before calling GetPublicKey.
    return CryptoPP::RSA::PublicKey();
}
