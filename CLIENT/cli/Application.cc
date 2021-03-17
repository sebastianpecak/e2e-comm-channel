#include "Application.h"
#include <iostream>
#include <sstream>
// Include all needed interface headers.
#include "PlainTextMessage.pb.h"
#include "SourceMessage.pb.h"
#include "TargetMessage.pb.h"
#include "ServerInterface.pb.h"
#include "AllMessages.pb.h"
#include "ConfirmDelivery.pb.h"
// Include cypto++.
#include <cryptopp/sha.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/pssr.h>
#include <cryptopp/aes.h>
#include <cryptopp/default.h>
// Include all needed header for UDP socket.
#include <netinet/in.h>
#include <netinet/udp.h>
#include <unistd.h>

#define LOG_ERROR(message) \
    fprintf(stderr, "ERR: %s\n", (message))

Application::Application(int argc, char **argv) :
    _config(),
    _needToStop(false)
{
    _config.ParseCliArguments(argc, argv);
}

int Application::Run()
{
    std::cout << "Enter user name: ";
    std::cin >> _userName;
    std::cin.ignore();
    // TODO: user name should be hashed.
    while (not _needToStop)
    {
        std::string command;
        printf("$> ");
        std::getline(std::cin, command);
        _ProcessCommand(command);
    }
    return 0;
}

void Application::_Initialize()
{
    // Open main application socket and bind it.
    // _mainSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    // if (_mainSocket == -1)
    // {
    //     std::cerr << "Failed to create UDP socket, due to: " << errno << std::endl;
    //     return;
    // }
    // sockaddr_in svrAddr = sockaddr_in();
    // svrAddr.sin_family = AF_INET;
    // svrAddr.sin_port = htons(1111);
    // svrAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // const int bindResult = bind(_mainSocket, reinterpret_cast<sockaddr*>(&svrAddr), sizeof(svrAddr));
    // if (bindResult == -1)
    // {
    //     std::cerr << "Failed to bind UDP socket, due to: " << errno << std::endl;
    //     return;
    // }
}

void Application::_ProcessCommand(const std::string &command)
{
    if (command.find("help") != std::string::npos)
    {
        _DisplayHelp();
    }
    else if (command.find("send") != std::string::npos)
    {
        _Send(command);
    }
    else if (command.find("recv") != std::string::npos)
    {
        _Recv(command);
    }
    else if (command.find("svrinfo") != std::string::npos)
    {
        _SvrInfo(command);
    }
    else if (command.find("exit") != std::string::npos)
    {
        _Exit(command);
    }
}

void Application::_Send(const std::string &command) const
{
    // Get recipient id from command.
    const auto separator = command.find(' ');
    const auto recipient = command.substr(separator + 1);
    printf("Sending mesage to %s\n", recipient.c_str());
    printf("Enter your message below. End it by typing enter.enter\n");
    // Read users message.
    std::stringstream builder;
    std::string buffer;
    bool isFirstLine = true;
    while (true)
    {
        std::getline(std::cin, buffer);
        // Check if end of message.
        if (buffer == ".")
        {
            break;
        }
        if (not isFirstLine)
        {
            builder << '\n';
        }
        else{
            isFirstLine = false;
        }
        builder << buffer;
    }
    printf("--- YOUR MESSAGE ---\n%s\n--- YOUR MESSAGE END---\n", builder.str().c_str());

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // NEED TO REMEMBER TO USE POINTER + SIZE OVERLOADS WITH BINARY DATA.
    // OTHERWISE OPERATIONS MAY FAIL.
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // Build message structure.
    PlainTextMessage plainTextMessage = PlainTextMessage();
    plainTextMessage.set_text(builder.str());
    const auto serializedPlainText = plainTextMessage.SerializeAsString();

    // Random pool to use across methods.
    CryptoPP::AutoSeededRandomPool rng;

    // Make source RSA key.
    CryptoPP::RSA::PrivateKey sourcePrivateKey;
    sourcePrivateKey.Initialize(rng, 2048);
    CryptoPP::RSA::PublicKey sourcePublicKey(sourcePrivateKey);

    // Sign source message.
    CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Signer signer(sourcePrivateKey);
    std::string signature;
    CryptoPP::StringSource(
        (CryptoPP::byte*)serializedPlainText.data(), serializedPlainText.size(), true, 
        new CryptoPP::SignerFilter(
            rng,
            signer,
            new CryptoPP::StringSink(signature)
        )
    );

    // Verify signature.
    CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Verifier verifier(sourcePublicKey);
    const auto signedMsg = serializedPlainText + signature;
    std::string recovered;
    try
    {
        CryptoPP::StringSource(
            (CryptoPP::byte*)signedMsg.data(), signedMsg.size(), true,
            new CryptoPP::SignatureVerificationFilter(
                verifier,
                new CryptoPP::StringSink(recovered),
                CryptoPP::SignatureVerificationFilter::THROW_EXCEPTION |
                CryptoPP::SignatureVerificationFilter::PUT_MESSAGE
            )
        );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    SourceMessage srcMsg = SourceMessage();
    srcMsg.set_sourceid(_userName);
    srcMsg.set_signedmsg(serializedPlainText+signature);

    // Make AES key.
    CryptoPP::SecByteBlock aesKey = CryptoPP::SecByteBlock(CryptoPP::AES::MAX_KEYLENGTH);
    rng.GenerateBlock(aesKey, aesKey.size());

    // Serialize and encrypt source message with AES key.
    const auto serialiedSrcMsg = srcMsg.SerializeAsString();
    std::string encSrcMsg;
    CryptoPP::StringSource(
        (CryptoPP::byte*)serialiedSrcMsg.data(), serialiedSrcMsg.size(), true,
        new CryptoPP::DefaultEncryptorWithMAC(
            (CryptoPP::byte*)aesKey.data(), aesKey.size(),
            //new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(encSrcMsg)
            //)
        )
    );

    // VERIFY !!!!!!!!!
    // Decrypt and deserialize and check if src msg is the same as decrypted one.
    std::string decSrcMsg;
    CryptoPP::StringSource(
        (CryptoPP::byte*)encSrcMsg.data(), encSrcMsg.size(), true,
        //new CryptoPP::HexDecoder(
        new CryptoPP::DefaultDecryptorWithMAC(
            (CryptoPP::byte*)aesKey.data(), aesKey.size(),
            new CryptoPP::StringSink(decSrcMsg)
        )
        //)
    );
    SourceMessage deserializedSrcMessage;
    deserializedSrcMessage.ParseFromString(decSrcMsg);
    if (deserializedSrcMessage.sourceid() != srcMsg.sourceid() || deserializedSrcMessage.signedmsg() != srcMsg.signedmsg())
    {
        std::cerr << "INVALID!" << std::endl;
    }
    // VERIFY !!!!!!!!!

    // Make target RSA key.
    CryptoPP::RSA::PrivateKey targetPrivateKey;
    targetPrivateKey.Initialize(rng, 2048);
    CryptoPP::RSA::PublicKey targetPublicKey(targetPrivateKey);

    // Encrypt AES key with target's public key.
    CryptoPP::RSAES_OAEP_SHA_Encryptor aesKeyEncryptor(targetPublicKey);
    std::string encSymetricKey;
    CryptoPP::StringSource(
        aesKey.data(), aesKey.size(), true,
        new CryptoPP::PK_EncryptorFilter(
            rng,
            aesKeyEncryptor,
            //new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(encSymetricKey)
            //)
        )
    );

    // VERIFY !!!!!!!!!
    // Decrypt.
    CryptoPP::RSAES_OAEP_SHA_Decryptor aesKeyDecryptor(targetPrivateKey);
    std::string decSymetricKey;
    CryptoPP::StringSource(
        (CryptoPP::byte*)encSymetricKey.data(), encSymetricKey.size(), true,
        //new CryptoPP::HexDecoder(
        new CryptoPP::PK_DecryptorFilter(
            rng,
            aesKeyDecryptor,
            new CryptoPP::StringSink(decSymetricKey)
        )
        //)
    );
    if (memcmp(aesKey.data(), decSymetricKey.data(), aesKey.size()) != 0)
    {
        std::cerr << "INVALID" << std::endl;
    }
    // VERIFY !!!!!!!!!

    // Make target message.
    TargetMessage targetMsg;
    targetMsg.set_targetid(recipient);
    targetMsg.set_encsymetrickey(encSymetricKey);
    targetMsg.set_encdata(encSrcMsg);

    // Make server interface message.
    ServerRequest request;
    request.set_type(ServerRequestType::SEND_MSG);
    request.set_data(targetMsg.SerializeAsString());

    // Now serialize request and send it to the server.
    const auto serializedRequest = request.SerializeAsString();
    std::cout << serializedRequest.size() << " " << serializedRequest.length() << std::endl;

    // Send TCP message to server.
    const int tcpSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcpSocket == -1)
    {
        std::cerr << "Failed to open TCP socket, due to: " << errno << std::endl;
        return;
    }
    sockaddr_in serverAddress     = sockaddr_in();
    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_port        = htons(1111);
    serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // Try to connect to server.
    if (connect(tcpSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        std::cerr << "Failed to connect TCP socket, due to: " << errno << std::endl;
        return;
    }
    const ssize_t result = send(tcpSocket, serializedRequest.data(), serializedRequest.size(), 0);
    //const ssize_t result = sendto(udpSocket, serializedRequest.data(), serializedRequest.size(), 0, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    if (result < serializedRequest.size())
    {
        std::cerr << "Sent " << result << " bytes, errno " << errno << std::endl;
        return;
    }
    // Wait for server response.
    unsigned char replyBuffer[4096];
    const ssize_t recvResult = recv(tcpSocket, replyBuffer, sizeof(replyBuffer), 0);
    if (recvResult == -1)
    {
        std::cerr << "Failed to receive TCP socket, due to: " << errno << std::endl;
        return;
    }
    // Parse response.
    ServerReply svrReply;
    if (not svrReply.ParseFromArray(replyBuffer, recvResult))
    {
        std::cerr << "Failed to parse server response." << std::endl;
        return;
    }
    std::cout << "Server replied with " << svrReply.reply() << std::endl;
    shutdown(tcpSocket, SHUT_RDWR);
    close(tcpSocket);
    return;
}

void Application::_Recv(const std::string &command) const
{
    printf("Receiving mesage for %s\n", _userName.c_str());
    // Make server interface message.
    ServerRequest request;
    request.set_type(ServerRequestType::RECV_ALL_MSG);
    AllMessagesRequest allMsgReq;
    allMsgReq.set_userid(_userName);
    request.set_data(allMsgReq.SerializeAsString());
    // Serialize whole request.
    const auto serializedRequest = request.SerializeAsString();

    // Send TCP message to server.
    const int tcpSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcpSocket == -1)
    {
        std::cerr << "Failed to open TCP socket, due to: " << errno << std::endl;
        return;
    }
    sockaddr_in serverAddress     = sockaddr_in();
    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_port        = htons(1111);
    serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // Try to connect to server.
    if (connect(tcpSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        std::cerr << "Failed to connect TCP socket, due to: " << errno << std::endl;
        return;
    }
    const ssize_t result = send(tcpSocket, serializedRequest.data(), serializedRequest.size(), 0);
    //const ssize_t result = sendto(udpSocket, serializedRequest.data(), serializedRequest.size(), 0, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    if (result < serializedRequest.size())
    {
        std::cerr << "Sent " << result << " bytes, errno " << errno << std::endl;
        return;
    }
    // Wait for server response.
    unsigned char replyBuffer[4096];
    const ssize_t recvResult = recv(tcpSocket, replyBuffer, sizeof(replyBuffer), 0);
    if (recvResult == -1)
    {
        std::cerr << "Failed to receive TCP socket, due to: " << errno << std::endl;
        return;
    }
    // Parse response.
    ServerReply svrReply;
    if (not svrReply.ParseFromArray(replyBuffer, recvResult))
    {
        std::cerr << "Failed to parse server response." << std::endl;
        return;
    }
    std::cout << "Server replied with " << svrReply.reply() << std::endl;
    AllMessagesReply allMsg;
    if (not allMsg.ParseFromString(svrReply.data()))
    {
        std::cerr << "Failed to parse AllMessagesReply." << std::endl;
        return;
    }
    // Make request to confirm delivery of messages.
    if (allMsg.messages_size() > 0)
    {
        ServerRequest confirmReq;
        confirmReq.set_type(ServerRequestType::CONFIRM_DELIVERY);
        ConfirmDelivery confirmMsg;
        confirmMsg.set_userid(_userName);
        // For each message.
        for (int i = 0; i < allMsg.messages_size(); ++i)
        {
            printf("NEW: Received message of ID %s\n", allMsg.messages(i).id().c_str());
            confirmMsg.add_messageid(allMsg.messages(i).id());
        }
        confirmReq.set_data(confirmMsg.SerializeAsString());
        const auto serializedMsgDel = confirmReq.SerializeAsString();
        const ssize_t result2 = send(tcpSocket, serializedMsgDel.data(), serializedMsgDel.size(), 0);
        //const ssize_t result = sendto(udpSocket, serializedRequest.data(), serializedRequest.size(), 0, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
        if (result2 < serializedMsgDel.size())
        {
            std::cerr << "Sent " << result2 << " bytes, errno " << errno << std::endl;
            return;
        }
        // Wait for server response.
        const ssize_t recvResult2 = recv(tcpSocket, replyBuffer, sizeof(replyBuffer), 0);
        if (recvResult2 == -1)
        {
            std::cerr << "Failed to receive TCP socket, due to: " << errno << std::endl;
            return;
        }
        // Parse response.
        ServerReply svrReply2;
        if (not svrReply2.ParseFromArray(replyBuffer, recvResult2))
        {
            std::cerr << "Failed to parse server response." << std::endl;
            return;
        }
        std::cout << "Server replied with " << svrReply2.reply() << std::endl;
    }
    shutdown(tcpSocket, SHUT_RDWR);
    close(tcpSocket);
    return;
}

void Application::_SvrInfo(const std::string &command) const
{
}

void Application::_Exit(const std::string &command)
{
    printf("Exiting gracefully\n");
    _needToStop = true;
}

void Application::_DisplayHelp() const
{
    printf("Options:\n");
    printf("    send recipient_id - send new message\n");
    printf("    recv              - receive all messages from server\n");
    printf("    help              - display all commands\n");
    printf("    exit              - exit gracefully\n");
}
