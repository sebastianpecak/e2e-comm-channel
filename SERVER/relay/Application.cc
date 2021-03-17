#include "Application.h"
#include <netinet/in.h>
#include <netinet/udp.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>
// INTERFACE library.
#include "ServerInterface.pb.h"
#include "TargetMessage.pb.h"
#include "SourceMessage.pb.h"
// Request processors.
#include "SendMsgRequest.h"
#include "RecvAllMsgRequest.h"
#include "ConfirmDeliveryRequest.h"


Application::Application(int argc, char **argv)
{
    _storage = new Storage();
}

int Application::Run()
{
    _mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_mainSocket == -1)
    {
        return -1;
    }
    sockaddr_in svrAddr = sockaddr_in();
    svrAddr.sin_family = AF_INET;
    svrAddr.sin_port = htons(1111);
    svrAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    const int bindResult = bind(_mainSocket, reinterpret_cast<sockaddr*>(&svrAddr), sizeof(svrAddr));
    if (bindResult == -1)
    {
        return -1;
    }
    unsigned char rcvData[1<<12];// memset(buffer, 0, sizeof(buffer));
    sockaddr_in peerAddr = sockaddr_in();
    socklen_t peerSockLen = 0;
    const int listenResult = listen(_mainSocket, 0);
    if (listenResult != 0)
    {
       fprintf(stderr, "Failed to listen %d.\n", errno);
        return -1;
    }
    while (true)
    {
        const int cliSocket = accept(_mainSocket, reinterpret_cast<sockaddr*>(&peerAddr), &peerSockLen);
        if (cliSocket < 0)
        {
            fprintf(stderr, "Failed to accept %d.\n", errno);
            continue;
        }
        while (true)
        {
            const ssize_t recvRes = recv(cliSocket, rcvData, sizeof(rcvData), 0);
            if (recvRes <= 0)
            {
                break;
            }
            //const ssize_t recvRes = recvfrom(_mainSocket, rcvData, sizeof(rcvData), 0, reinterpret_cast<sockaddr*>(&peerAddr), &peerSockLen);
            //if (recvRes > 0)
            //{
            _ProcessRequest(rcvData, recvRes, peerAddr, cliSocket);
            //}
        }
        shutdown( cliSocket, SHUT_RDWR );
        close(cliSocket);
    }
}

void Application::_ProcessRequest(const void *data, unsigned int msgSize, const sockaddr_in &peer, int cliSocket)
{
    printf("New request received. Processing it...\n");
    // Try to deserialize message.
    ServerRequest request;
    if (not request.ParseFromArray(data, msgSize))
    {
        fprintf(stderr, "Failed to parse request.");
        return;
    }
    // Check request type.
    if (request.type() == ServerRequestType::SEND_MSG)
    {
        SendMsgRequest processor = SendMsgRequest(_storage, request, peer);
        processor.Process();
        _SendReply(processor, cliSocket);
    }
    else if (request.type() == ServerRequestType::GET_SVR_INFO)
    {
        
    }
    else if (request.type() == ServerRequestType::RECV_ALL_MSG)
    {
        RecvAllMsgRequest processor = RecvAllMsgRequest(_storage, request, peer);
        processor.Process();
        _SendReply(processor, cliSocket);
    }
    else if (request.type() == ServerRequestType::CONFIRM_DELIVERY)
    {
        ConfirmDeliveryRequest processor = ConfirmDeliveryRequest(_storage, request, peer);
        processor.Process();
        _SendReply(processor, cliSocket);
    }
    else
    {
        fprintf(stderr, "Unknown request type %d.\n", static_cast<int>(request.type()));
    }
    //printf("Finished processing new request.\n");
}

void Application::_SendReply(const IRequest &request, int cliSocket)
{
    const auto serializedReply = request.GetReply().SerializeAsString();
    const ssize_t result = send(cliSocket, serializedReply.data(), serializedReply.size(), 0);
    //const ssize_t result = sendto(_mainSocket, serializedReply.data(), serializedReply.size(), MSG_DONTROUTE, reinterpret_cast<const sockaddr*>(&request.GetPeer()), sizeof(sockaddr_in));
    if (result < serializedReply.size())
    {
        std::cerr << "Sent " << result << " bytes, errno " << errno << std::endl;
    }
}
