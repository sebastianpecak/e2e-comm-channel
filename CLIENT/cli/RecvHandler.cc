#include "RecvHandler.h"
#include <cryptopp/rsa.h>
#include <cryptopp/pssr.h>
#include <cryptopp/aes.h>
#include <cryptopp/default.h>
#include "SourceMessage.pb.h"
#include "PlainTextMessage.pb.h"

RecvHandler::RecvHandler(Session *session, UserKeyStore *keyStore, ServerInterface *svrIface) :
    _session(session),
    _keyStore(keyStore),
    _svrIface(svrIface)
{
}

void RecvHandler::Execute(const Tokenizer &tokens)
{
    using namespace CryptoPP;

    // There must be a valid user's session.
    if (not _session->IsActive())
    {
        std::cerr << "You must login first." << std::endl;
        return;
    }
    // Try to obtain all new user's messages from server.
    AllMessagesReply allMsg;
    if (not _svrIface->GetAllMessages(_session->GetId(), allMsg))
    {
        std::cerr << "Failed to get all messages from server." << std::endl;
        return;
    }
    // Return right away if there is new message.
    if (allMsg.messages_size() == 0)
    {
        std::cout << "There are no new message for you." << std::endl;
        return;
    }
    // Make request to confirm delivery of messages.
    DeliveryResult deliveryMsg;
    deliveryMsg.set_userid(_session->GetId());
    // For each message.
    for (int i = 0; i < allMsg.messages_size(); ++i)
    {
        const auto &ftm = allMsg.messages(i);
        const auto &tm  = ftm.msg();
        // Decrypt symetric key.
        RSAES_OAEP_SHA_Decryptor aesKeyDecryptor(_session->GetKey());
        SafeString aesKey;
        try
        {
            StringSource(
                reinterpret_cast<const byte*>(tm.encsymetrickey().data()),
                tm.encsymetrickey().size(),
                true,
                new PK_DecryptorFilter(
                    _rng,
                    aesKeyDecryptor,
                    new StringSink(aesKey.str())
                )
            );
        }
        catch (const std::exception &e)
        {
            (*deliveryMsg.mutable_msgstatus())[ftm.id()] = DeliveryStatus::MSG_NOK;
            std::cerr << "Failed to decrypt symetric key, due to: " << e.what() << std::endl;
            continue;
        }
        // Decrypt and deserialize source message.
        std::string decSrcMsg;
        try
        {
            StringSource(
                reinterpret_cast<const byte*>(tm.encdata().data()),
                tm.encdata().size(),
                true,
                new DefaultDecryptorWithMAC(
                    reinterpret_cast<const byte*>(aesKey.str().data()),
                    aesKey.str().size(),
                    new StringSink(decSrcMsg)
                )
            );
        }
        catch (const std::exception &e)
        {
            (*deliveryMsg.mutable_msgstatus())[ftm.id()] = DeliveryStatus::MSG_NOK;
            std::cerr << "Failed to decrypt message " << ftm.id() << ", due to: " << e.what() << std::endl;
            continue;
        }
        SourceMessage srcMsg;
        if (not srcMsg.ParseFromString(decSrcMsg))
        {
            (*deliveryMsg.mutable_msgstatus())[ftm.id()] = DeliveryStatus::MSG_NOK;
            std::cerr << "Failed to deserialize message " << ftm.id() << std::endl;
            continue;
        }
        // Verify signature of source message.
        RSASS<PKCS1v15, SHA256>::Verifier verifier(_keyStore->GetPublicKey(srcMsg.sourceid()));
        std::string serializedMsg;
        try
        {
            StringSource(
                reinterpret_cast<const byte*>(srcMsg.signedmsg().data()),
                srcMsg.signedmsg().size(),
                true,
                new SignatureVerificationFilter(
                    verifier,
                    new StringSink(serializedMsg),
                    SignatureVerificationFilter::THROW_EXCEPTION | SignatureVerificationFilter::PUT_MESSAGE
                )
            );
        }
        catch (const std::exception &e)
        {
            (*deliveryMsg.mutable_msgstatus())[ftm.id()] = DeliveryStatus::MSG_NOK;
            std::cerr << e.what() << std::endl;
            continue;
        }
        // Parse plain text message.
        PlainTextMessage ptm;
        if (not ptm.ParseFromString(serializedMsg))
        {
            (*deliveryMsg.mutable_msgstatus())[ftm.id()] = DeliveryStatus::MSG_NOK;
            std::cerr << "Failed to deserialize PlainTextMessage " << ftm.id() << std::endl;
            continue;
        }
        // Print this message to user.
        (*deliveryMsg.mutable_msgstatus())[ftm.id()] = DeliveryStatus::MSG_OK;
        std::cout << "New message <" << ftm.id() << "> from <" << srcMsg.sourceid() << ">:" << std::endl << ptm.text() << std::endl;
    }
    // Try to send delivery result message to server.
    if (not _svrIface->SendDeliveryResult(deliveryMsg))
    {
        std::cerr << "Failed to send delivery result message to server." << std::endl;
        return;
    }
}
