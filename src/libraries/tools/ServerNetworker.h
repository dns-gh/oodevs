// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ServerNetworker_h_
#define __ServerNetworker_h_

#include "MessageDispatcher_ABC.h"
#include "MessageSender_ABC.h"
#include "ConnectionCallback_ABC.h"
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )
#include <string>
#include <memory>

namespace boost
{
namespace asio
{
    class io_service;
}
}

namespace tools
{
    class ObjectMessageService;
    class ConnectionCallback_ABC;
    class BufferedMessageCallback;
    class SocketManager;
    class Acceptor;
    class WaitEvent;

// =============================================================================
/** @class  ServerNetworker
    @brief  ServerNetworker
*/
// Created: NLD 2006-09-19
// =============================================================================
class ServerNetworker : public MessageDispatcher_ABC
                      , public MessageSender_ABC
                      , private ConnectionCallback_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ServerNetworker( const std::string& endpoint, unsigned long timeOut = 10000 );
    virtual ~ServerNetworker();
    //@}

    //! @name Operations
    //@{
    void Update();

    void Disconnect( const std::string& endpoint );
    void DenyConnections();
    void AllowConnections();

    using MessageSender_ABC::Send;
    using MessageDispatcher_ABC::RegisterMessage;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetNbMessagesSent() const;
    virtual unsigned long GetNbMessagesReceived() const;
    virtual bool HasAnsweredSinceLastTick( const std::string& endpoint );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& local, const std::string& remote );
    virtual void ConnectionFailed( const std::string& address, const std::string& error );
    virtual void ConnectionError( const std::string& address, const std::string& error );
    virtual void ConnectionWarning( const std::string& address, const std::string& warning );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Send( const std::string& endpoint, unsigned long tag, Message& message );
    virtual void Register( unsigned long id, std::unique_ptr< ObjectMessageCallback_ABC > callback );
    virtual ObjectMessageCallback_ABC* Retrieve( unsigned long id );
    //@}

    //! @name Helpers
    //@{
    void Run();
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< boost::asio::io_service >      service_;
    boost::shared_ptr< BufferedMessageCallback >    messageBuffer_;
    std::unique_ptr< SocketManager >                sockets_;
    std::unique_ptr< ObjectMessageService >         messageService_;
    std::unique_ptr< Acceptor >                     acceptor_;
    std::unique_ptr< WaitEvent >                    quit_;
    boost::thread                                   thread_;
    //@}
};

}

#endif // __ServerNetworker_h_
