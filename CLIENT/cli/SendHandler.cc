#include "SendHandler.h"
#include <sstream>
#include "PlainTextMessage.pb.h"
#include "SourceMessage.pb.h"
#include <cryptopp/rsa.h>
#include <cryptopp/pssr.h>
#include <cryptopp/aes.h>
#include <cryptopp/default.h>

static constexpr char END_OF_FILE = 0x4;

SendHandler::SendHandler(Session *session, UserKeyStore *keyStore, ServerInterface *svrIface) :
    _session(session),
    _keyStore(keyStore),
    _svrIface(svrIface)
{
}

void SendHandler::Execute(const Tokenizer &tokens)
{
    using namespace CryptoPP;

    // There must be a valid user's session.
    if (not _session->IsActive())
    {
        std::cerr << "You must login first." << std::endl;
        return;
    }
    // Second token must be target user id.
    if (tokens.GetTokensCount() < 2)
    {
        std::cerr << "You must specify target id." << std::endl;
        return;
    }
    // Target user's public key must be in database.
    if (not _keyStore->IsUserKnown(tokens.GetToken(1)))
    {
        std::cerr << "You do not have recipient's public key and cannot send message to them." << std::endl;
        return;
    }
    // Read user's message.
    const auto userMessage = _GetUserMessage();
    // Build message structure and serialize it.
    PlainTextMessage plainTextMessage;
    plainTextMessage.set_text(userMessage);
    const auto sptm = plainTextMessage.SerializeAsString();
    // Sign source message.
    RSASS<PSS, SHA256>::Signer signer(_session->GetKey());
    std::string signature;
    StringSource(
        reinterpret_cast<const byte*>(sptm.data()),
        sptm.size(),
        true, 
        new SignerFilter(
            _rng,
            signer,
            new StringSink(signature)
        )
    );
    // Concatenate message with signature.
    const auto signedMsg = sptm + signature;
    // Make source message.
    SourceMessage srcMsg = SourceMessage();
    srcMsg.set_sourceid(_session->GetId());
    srcMsg.set_signedmsg(signedMsg);
    // Generate random AES key.
    SecByteBlock aesKey = SecByteBlock(AES::MAX_KEYLENGTH);
    _rng.GenerateBlock(aesKey, aesKey.size());
    // Serialize and encrypt source message with AES key.
    const auto ssm = srcMsg.SerializeAsString();
    std::string encSrcMsg;
    StringSource(
        reinterpret_cast<const byte*>(ssm.data()),
        ssm.size(),
        true,
        new DefaultEncryptorWithMAC(
            reinterpret_cast<const byte*>(aesKey.data()),
            aesKey.size(),
            new StringSink(encSrcMsg)
        )
    );
    // Encrypt AES key with target's public key.
    RSAES_OAEP_SHA_Encryptor aesKeyEncryptor(_keyStore->GetPublicKey(tokens.GetToken(1)));
    std::string encSymetricKey;
    StringSource(
        aesKey.data(),
        aesKey.size(),
        true,
        new PK_EncryptorFilter(
            _rng,
            aesKeyEncryptor,
            new StringSink(encSymetricKey)
        )
    );
    // Make target message.
    TargetMessage targetMsg;
    targetMsg.set_targetid(tokens.GetToken(1));
    targetMsg.set_encsymetrickey(encSymetricKey);
    targetMsg.set_encdata(encSrcMsg);
    // Try to send new message to server.
    if (not _svrIface->SendMessage(targetMsg))
    {
        std::cerr << "Failed to send message." << std::endl;
        return;
    }
    // Success.
    std::cout << "Message was successfully sent." << std::endl;
}

std::string SendHandler::_GetUserMessage() const
{
    std::cout << "Enter your message below. To finish press CTRL+D:" << std::endl;
    // Read multi-line message.
    std::string message;
    std::getline(std::cin, message, END_OF_FILE).clear();
    // Put new-line character to stdout if one-before-last character was not new-line.
    if (message.length() > 1 && message[message.length() - 2] != '\n')
    {
        static_cast<void>(putchar('\n'));
    }
    return message;
}
