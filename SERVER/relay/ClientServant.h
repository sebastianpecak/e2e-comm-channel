#ifndef CLIENTSERVANT_H
#define CLIENTSERVANT_H

#include "ClientConnection.h"
#include "ClassLog.h"
#include "ServerInterface.pb.h"
#include "IRequest.h"
#include "Storage.h"
#include <memory>

/**
 * This class is used to process all clients requests.
 * It can serve clients in first-in-first-served manner or parallel manner.
 */
class ClientServant
{
private:
    /**
     * Logger instance.
     */
    ClassLog _log;
    /**
     * Storage instance.
     */
    Storage *_storage;

    /**
     * General function called when new request is received from client.
     * Returns true if request was successfully handled.
     */
    bool _ProcessRequest(ClientConnection &connection, const ServerRequest &request);
    /**
     * Returns proper request handler object.
     * Implementation of strategy pattern.
     * Returns nullptr if there is no handler for given request type.
     */
    std::unique_ptr<IRequest> _GetRequestHandler(ServerRequestType type, const ServerRequest &request) const;

public:
    /**
     * Basic ctor and dtor.
     */
    explicit ClientServant(Storage *storage);
    ~ClientServant();
    /**
     * Process new client's connection.
     * For now it processes clients sequentially.
     * It could be changed in the future to process clients in parallel.
     */
    void Serve(ClientConnection &&connection);
};

#endif // CLIENTSERVANT_H
