#include "GetSvrInfoRequest.h"
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>

#define SERVER_NAME    "e2e_svr_relay_"
#define SERVER_VERSION "linux_0.0.1_"

GetSvrInfoRequest::GetSvrInfoRequest(Storage *storage, const ServerRequest &request) :
    _storage(storage),
    _request(request),
    _isProcessed(false)
{
}

void GetSvrInfoRequest::Process()
{
    using namespace CryptoPP;

    // Make a random string.
    AutoSeededRandomPool rng;
    byte custom[4];
    rng.GenerateBlock(custom, sizeof(custom));
    std::string customString;
    StringSource(
        custom,
        sizeof(custom),
        true,
        new HexEncoder(
            new StringSink(
                customString
            )
        )
    );
    _info.set_name(SERVER_NAME + customString);
    _info.set_version(SERVER_VERSION + customString);
    _isProcessed = true;
}

ServerReply GetSvrInfoRequest::GetReply() const
{
    ServerReply reply;
    reply.set_type(_request.type());
    reply.set_reply(_isProcessed ? ServerReplyCode::SUCCESS : ServerReplyCode::FAILURE);
    reply.set_data(_info.SerializeAsString());
    return reply;
}
