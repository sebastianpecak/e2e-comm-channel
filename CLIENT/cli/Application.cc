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
#include "ServerInfo.pb.h"
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
#include <termios.h>

#include "SafeString.h"

#define LOG_ERROR(message) \
    fprintf(stderr, "ERR: %s\n", (message))

Application::Application(int argc, char **argv) :
    _config(),
    _needToStop(false)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    _config.ParseCliArguments(argc, argv);
}

Application::~Application()
{
    // Make sure to logout user.
    _Logout("");
}

int Application::Run()
{
    //std::cout << "Enter user name: ";
    //std::cin >> _userName;
    //std::cin.ignore();
    //_GenerateKeyIfNeeded(_userName);
    // TODO: user name should be hashed.
    while (not _needToStop)
    {
        std::string command;
        printf("%s:~$ ", _userName.c_str());
        std::getline(std::cin, command);
        _ProcessCommand(command);
    }
    return 0;
}

void Application::_Initialize()
{
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
    else if (command.find("svr-info") != std::string::npos)
    {
        _SvrInfo(command);
    }
    else if (command.find("exit") != std::string::npos)
    {
        _Exit(command);
    }
    else if (command.find("login") != std::string::npos)
    {
        _Login(command);
    }
    else if (command.find("create-user") != std::string::npos)
    {
        _CreateUser(command);
    }
    else if (command.find("export-user") != std::string::npos)
    {
        _ExportUser(command);
    }
    else if (command.find("import-user") != std::string::npos)
    {
        _ImportUser(command);
    }
    else if (command.find("logout") != std::string::npos)
    {
        _Logout(command);
    }
    else
    {
        std::cout << "Unkown command " << command << std::endl;
    }
}

void Application::_ExportUser(const std::string &command)
{
    const auto separator = command.find(' ');
    const auto userId = command.substr(separator + 1);
    if (separator == std::string::npos || userId.length() == 0)
    {
        std::cerr << "You must specify target id." << std::endl;
        return;
    }
    if (not _keyStore.IsUserKnown(userId))
    {
        std::cerr << "There is no such user." << std::endl;
        return;
    }
    std::cout << "COMPACT-RECORD: " << _keyStore.ExportCompactRecord(userId) << std::endl;
}

void Application::_ImportUser(const std::string &command)
{
    const auto separator = command.find(' ');
    const auto compactRecord = command.substr(separator + 1);
    if (separator == std::string::npos || compactRecord.length() == 0)
    {
        std::cerr << "Missing compact-record value." << std::endl;
        return;
    }
    if (not _keyStore.ImportCompactRecord(compactRecord))
    {
        std::cerr << "Failed to import user from compact-record." << std::endl;
        return;
    }
}

void Application::_Logout(const std::string &command)
{
    if (_userKey != nullptr)
    {
        _userName.clear();
        _userKey.reset();
        std::cout << "User logged out." << std::endl;
    }
    else
    {
        std::cout << "No user was logged in." << std::endl;
    }
}




void Application::_Login(const std::string &command)
{
    // Check if there is no user logged in.
    if (_userKey != nullptr)
    {
        std::cerr << "Need to logout first." << std::endl;
        return;
    }
    const auto separator = command.find(' ');
    _userName = command.substr(separator + 1);
    if (separator == std::string::npos || _userName.length() == 0)
    {
        _userName.clear();
        std::cerr << "Missing user id for login command." << std::endl;
        return;
    }
    // Check if user exists.
    if (not _keyStore.IsUserLoggable(_userName))
    {
        _userName.clear();
        std::cerr << "This user does not allow login." << std::endl;
        return;
    }
    SafeString password;
    //std::string password;
    std::cout << "Password: "; std::cout.flush();
    _SetTerminalPasswordMode(true);
    std::cin >> password.str();
    std::cin.ignore();
    _SetTerminalPasswordMode(false);
    _userKey = _keyStore.GetPrivateKey(_userName, password);
    if (_userKey == nullptr)
    {
        _userName.clear();
        std::cerr << std::endl << "Failed to login user. Probably invalid password." << std::endl;
        return;
    }
    std::cout << std::endl << "User successfully logged in." << std::endl;
}

void Application::_CreateUser(const std::string &command)
{
    const auto separator = command.find(' ');
    const auto userId = command.substr(separator + 1);
    if (separator == std::string::npos || userId.length() == 0)
    {
        std::cerr << "Missing user id for create command." << std::endl;
        return;
    }
    if (_keyStore.IsUserKnown(userId))
    {
        std::cerr << "This user already exists." << std::endl;
        return;
    }
    _SetTerminalPasswordMode(true);
    // ###############################3
    // All such places must be zeroed on scope exist to no leave raw keys/pwds in memory.
    // ###############################
    //std::string pwd, pwdAgain;
    SafeString pwd, pwdAgain;
    std::cout << "Password: "; std::cout.flush();
    std::cin >> pwd.str();
    std::cout << std::endl << "Again: "; std::cout.flush();
    std::cin >> pwdAgain.str();
    std::cin.ignore();
    _SetTerminalPasswordMode(false);
    // Check if both pwds are equal.
    if (pwd.str() != pwdAgain.str())
    {
        std::cerr << std::endl << "Passwords are not the same." << std::endl;
        return;
    }
    _keyStore.CreateUser(userId, pwd);
    std::cout << std::endl << "New user has been added." << std::endl;
}

void Application::_SetTerminalPasswordMode(bool enable)
{
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (enable)
    {
        tty.c_lflag &= ~ECHO;
    }
    else
    {
        tty.c_lflag |= ECHO;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void Application::_Send(const std::string &command)
{
    if (_userKey == nullptr)
    {
        std::cerr << "You must login first." << std::endl;
        return;
    }
    // Get recipient id from command.
    const auto separator = command.find(' ');
    const auto recipient = command.substr(separator + 1);
    if (separator == std::string::npos || recipient.length() == 0)
    {
        std::cerr << "You must specify target id." << std::endl;
        return;
    }
    if (not _keyStore.IsUserKnown(recipient))
    {
        std::cerr << "You do not have recipient's public key and cannot send message to them." << std::endl;
        return;
    }
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

    // Sign source message.
    CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Signer signer(*_userKey);
    std::string signature;
    CryptoPP::StringSource(
        (CryptoPP::byte*)serializedPlainText.data(), serializedPlainText.size(), true, 
        new CryptoPP::SignerFilter(
            rng,
            signer,
            new CryptoPP::StringSink(signature)
        )
    );
    // VERIFY !!!!!!!!!
    // Verify signature.
    // CryptoPP::RSA::PublicKey pubKey(*_userKey);
    // CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Verifier verifier(pubKey);
    const auto signedMsg = serializedPlainText + signature;
    // std::string recovered;
    // try
    // {
    //     CryptoPP::StringSource(
    //         (CryptoPP::byte*)signedMsg.data(), signedMsg.size(), true,
    //         new CryptoPP::SignatureVerificationFilter(
    //             verifier,
    //             new CryptoPP::StringSink(recovered),
    //             CryptoPP::SignatureVerificationFilter::THROW_EXCEPTION |
    //             CryptoPP::SignatureVerificationFilter::PUT_MESSAGE
    //         )
    //     );
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr << "EXCEPTION: " <<  e.what() << std::endl;
    //     return;
    // }
    // // Deserialize message and compare with original.
    // PlainTextMessage ptmVerify;
    // if (not ptmVerify.ParseFromString(recovered))
    // {
    //     std::cerr << "Failed to parse PlainTextMessage." << std::endl;
    //     return;
    // }
    // if (ptmVerify.text() != plainTextMessage.text())
    // {
    //     std::cerr << "VERIFY: messages are not the same." << std::endl;
    //     return;
    // }
    // VERIFY !!!!!!!!!

    SourceMessage srcMsg = SourceMessage();
    srcMsg.set_sourceid(_userName);
    srcMsg.set_signedmsg(signedMsg);

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
    // std::string decSrcMsg;
    // CryptoPP::StringSource(
    //     (CryptoPP::byte*)encSrcMsg.data(), encSrcMsg.size(), true,
    //     //new CryptoPP::HexDecoder(
    //     new CryptoPP::DefaultDecryptorWithMAC(
    //         (CryptoPP::byte*)aesKey.data(), aesKey.size(),
    //         new CryptoPP::StringSink(decSrcMsg)
    //     )
    //     //)
    // );
    // SourceMessage deserializedSrcMessage;
    // deserializedSrcMessage.ParseFromString(decSrcMsg);
    // if (deserializedSrcMessage.sourceid() != srcMsg.sourceid() || deserializedSrcMessage.signedmsg() != srcMsg.signedmsg())
    // {
    //     std::cerr << "INVALID!" << std::endl;
    // }
    // VERIFY !!!!!!!!!

    // Make target RSA key.
    //CryptoPP::RSA::PrivateKey targetPrivateKey;
    //targetPrivateKey.Initialize(rng, 2048);
    //CryptoPP::RSA::PublicKey targetPublicKey(targetPrivateKey);
    //_GenerateKeyIfNeeded(recipient);

    // Encrypt AES key with target's public key.
    CryptoPP::RSAES_OAEP_SHA_Encryptor aesKeyEncryptor(_keyStore.GetPublicKey(recipient)); //_GetPublicKey(recipient));
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
    /* DEBUG ONLY *///const auto recipientPrivKey = _keyStore.GetPrivateKey(recipient, "zaq1@WSX");
    // CryptoPP::RSAES_OAEP_SHA_Decryptor aesKeyDecryptor(*recipientPrivKey);//_GetPrivateKey(recipient));
    // std::string decSymetricKey;
    // CryptoPP::StringSource(
    //     (CryptoPP::byte*)encSymetricKey.data(), encSymetricKey.size(), true,
    //     //new CryptoPP::HexDecoder(
    //     new CryptoPP::PK_DecryptorFilter(
    //         rng,
    //         aesKeyDecryptor,
    //         new CryptoPP::StringSink(decSymetricKey)
    //     )
    //     //)
    // );
    // if (memcmp(aesKey.data(), decSymetricKey.data(), aesKey.size()) != 0)
    // {
    //     std::cerr << "INVALID" << std::endl;
    // }
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
    std::cout << "SERIALIZED REQUEST SIZE: " << serializedRequest.size() <<  std::endl;

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

void Application::_Recv(const std::string &command)
{
    if (_userKey == nullptr)
    {
        std::cerr << "You must login first." << std::endl;
        return;
    }
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
            const ForwardTargetMessage &ftg = allMsg.messages(i);
            printf("NEW: Received message of ID %s\n", ftg.id().c_str());
            confirmMsg.add_messageid(ftg.id());
            // Decrypt symetric key.
            CryptoPP::RSAES_OAEP_SHA_Decryptor aesKeyDecryptor(*_userKey);// _GetPrivateKey(_userName));
            std::string aesKey;
            CryptoPP::StringSource(
                (CryptoPP::byte*)ftg.msg().encsymetrickey().data(), ftg.msg().encsymetrickey().size(), true,
                new CryptoPP::PK_DecryptorFilter(
                    rng,
                    aesKeyDecryptor,
                    new CryptoPP::StringSink(aesKey)
                )
            );
            // Decrypt and deserialize and check if src msg is the same as decrypted one.
            std::string decSrcMsg;
            CryptoPP::StringSource(
                (CryptoPP::byte*)ftg.msg().encdata().data(), ftg.msg().encdata().size(), true,
                new CryptoPP::DefaultDecryptorWithMAC(
                    (CryptoPP::byte*)aesKey.data(), aesKey.size(),
                    new CryptoPP::StringSink(decSrcMsg)
                )
            );
            SourceMessage srcMsg;
            if (not srcMsg.ParseFromString(decSrcMsg))
            {
                std::cerr << "Failed to deserialize message " << ftg.id() << std::endl;
                continue;
            }
            // Verify signature.
            CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Verifier verifier(_keyStore.GetPublicKey(srcMsg.sourceid()));// _GetPublicKey(srcMsg.sourceid()));
            std::string serializedMsg;
            try
            {
                CryptoPP::StringSource(
                    (CryptoPP::byte*)srcMsg.signedmsg().data(), srcMsg.signedmsg().size(), true,
                    new CryptoPP::SignatureVerificationFilter(
                        verifier,
                        new CryptoPP::StringSink(serializedMsg),
                        CryptoPP::SignatureVerificationFilter::THROW_EXCEPTION |
                        CryptoPP::SignatureVerificationFilter::PUT_MESSAGE
                    )
                );
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                continue;
            }
            // Parse plain text message.
            PlainTextMessage ptm;
            if (not ptm.ParseFromString(serializedMsg))
            {
                std::cerr << "Failed to deserialize PlainTextMessage " << ftg.id() << std::endl;
                continue;
            }
            // Print this message to user.
            std::cout << "Message from <" << srcMsg.sourceid() << ">:" << std::endl << ptm.text() << std::endl;
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
    printf("Getting server info...\n");
    ServerRequest svrReq;
    svrReq.set_type(ServerRequestType::GET_SVR_INFO);
    const auto serializedReq = svrReq.SerializeAsString();
    std::cout << "size: " << serializedReq.size() << std::endl;

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
    const ssize_t result = send(tcpSocket, serializedReq.data(), serializedReq.size(), 0);
    if (result < serializedReq.size())
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
    ServerReply svrReply;
    if (not svrReply.ParseFromArray(replyBuffer, recvResult))
    {
        std::cerr << "Failed to parse server reply." << std::endl;
        return;
    }
    std::cout << "Server replied with " << svrReply.reply() << std::endl;
    ServerInfo svrInfo;
    if (not svrInfo.ParseFromString(svrReply.data()))
    {
        std::cerr << "Failed to parse server info." << std::endl;
        return;
    }
    std::cout << "SERVER NAME: " << svrInfo.name() << ", SERVER VERSION: " << svrInfo.version() << std::endl;
    shutdown(tcpSocket, SHUT_RDWR);
    close(tcpSocket);
    return;
}

void Application::_Exit(const std::string &command)
{
    printf("Exiting gracefully\n");
    _needToStop = true;
}

void Application::_DisplayHelp() const
{
    printf("Commands list:\n");
    printf("    login <user-id>              - Start new session as given user.\n");
    printf("    logout                       - End current user's session.\n");
    printf("    create-user <user-id>        - Create new user with gievn user-id.\n");
    printf("    export-user <user-id>        - Exports user's public key.\n");
    printf("    import-user <compact-record> - Imports user from compact-record.\n");
    printf("    send <user-id>               - Send new message to user-id.\n");
    printf("    recv                         - Get all current user's messages from server.\n");
    printf("    svr-info                     - Get server name and version.\n");
    printf("    help                         - Display commands list.\n");
    printf("    exit                         - Logout and exit gracefully.\n");
}
