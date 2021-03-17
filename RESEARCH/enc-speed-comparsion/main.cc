/**
 * This file implements encryption speed test (comparsion) of
 * symertic and asymetric algorithms.
 * We compare here RSA and AES.
 * Speed is measured for 1KB, 10KB, 100KB, 1MB, 10MB, 100MB data blocks.
 * CSV is generated in stdout.
 */

#include <iostream>
#include <chrono>
#include <string>
//#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/aes.h>
#include <cryptopp/default.h>

#define ONE_KB 1024
#define ONE_MB (ONE_KB * ONE_KB)
#define RSA_KEY_SIZE 2048
#define AES_KEY_SIZE 256

void _TestRSA(const void *data, unsigned int dataSize);
void _TestAES(const void *data, unsigned int dataSize);
void _PrintHeader();
void _PrintResult(const std::string &type, unsigned int keySize, unsigned int dataSize, const std::chrono::milliseconds &dur);

int main()
{
    const int blockSizes[] =
    {
        ONE_KB,
        ONE_KB * 10,
        ONE_KB * 100,
        ONE_MB,
        ONE_MB * 10,
        ONE_MB * 100
    };
    const int blocksCount = sizeof(blockSizes) / sizeof(blockSizes[0]);
    // Try to allocate data buffer. This data will be encrypted and we do not care about its contents.
    const unsigned char *dataBlock = new unsigned char[ONE_MB * 100];
    if (dataBlock == nullptr)
    {
        fprintf(stderr, "Failed to allocate 100MB memory block.\n");
        return -1;
    }

    _PrintHeader();

    for (int i = 0; i < blocksCount; ++i)
    {
        _TestRSA(dataBlock, blockSizes[i]);
        _TestAES(dataBlock, blockSizes[i]);
    }

    // Free allocated memory.
    delete[] dataBlock;
    dataBlock = nullptr;

    return 0;
}

void _TestRSA(const void *data, unsigned int dataSize)
{
    using namespace std::chrono;
    using namespace CryptoPP;

    // Prepare all needed objects.
    AutoSeededRandomPool rng;
    RSA::PrivateKey privateKey;
    privateKey.Initialize(rng, RSA_KEY_SIZE);
    RSAES_OAEP_SHA_Encryptor rsaEncryptor(privateKey);
    std::string encryptedData;
    // Start mesurement.
    const auto start = high_resolution_clock::now();
    // We have to divide large data block into smaller chunks of FixedMaxPlaintextLength bytes.
    for (size_t i = 0; i < dataSize; i += rsaEncryptor.FixedMaxPlaintextLength())
    {
        StringSource(
        reinterpret_cast<const byte*>(data) + i,
        rsaEncryptor.FixedMaxPlaintextLength(),
        true,
        new PK_EncryptorFilter(
            rng,
            rsaEncryptor,
            new StringSink(encryptedData)
        )
    );
    }
    // End mesurement.
    const auto end = high_resolution_clock::now();
    _PrintResult("RSA", RSA_KEY_SIZE, dataSize, duration_cast<milliseconds>(end - start));
}

void _TestAES(const void *data, unsigned int dataSize)
{
    using namespace std::chrono;
    using namespace CryptoPP;

    // Prepare all needed objects.
    AutoSeededRandomPool rng;
    SecByteBlock aesKey = SecByteBlock(AES::MAX_KEYLENGTH);
    rng.GenerateBlock(aesKey, aesKey.size());
    std::string encryptedData;
    // Start mesurement.
    const auto start = high_resolution_clock::now();
    StringSource(
        reinterpret_cast<const byte*>(data),
        dataSize,
        true,
        new DefaultEncryptorWithMAC(
            reinterpret_cast<const byte*>(aesKey.data()),
            aesKey.size(),
            new StringSink(encryptedData)
        )
    );
    // End mesurement.
    const auto end = high_resolution_clock::now();
    _PrintResult("AES", AES_KEY_SIZE, dataSize, duration_cast<milliseconds>(end - start));
}

void _PrintHeader()
{
    std::cout << "type;key_size(bits);data_block_size;duration(ms)" << std::endl;
}

void _PrintResult(const std::string &type, unsigned int keySize, unsigned int dataSize, const std::chrono::milliseconds &dur)
{
    std::cout << type << ';' << keySize << ';' << dataSize << ';' << dur.count() << std::endl;
}
