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
#pragma warning( push )
#pragma warning( disable : 4244 4512 )
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
    class BufferedConnectionCallback;
    class SocketManager;
    class Acceptor;

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
    explicit ServerNetworker( unsigned short port );
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
    //@}

protected:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ServerNetworker( const ServerNetworker& );            //!< Copy constructor
    ServerNetworker& operator=( const ServerNetworker& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void Send( const std::string& endpoint, unsigned long tag, const Message& message );
    virtual void Register( unsigned long id, std::auto_ptr< ObjectMessageCallback_ABC > callback );
    virtual ObjectMessageCallback_ABC* Retrieve( unsigned long id );
    //@}

    //! @name Helpers
    //@{
    void Run();
    void Stop();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< boost::asio::io_service >    service_;
    std::auto_ptr< BufferedConnectionCallback > connectionBuffer_;
    std::auto_ptr< BufferedMessageCallback >    messageBuffer_;
    std::auto_ptr< SocketManager >              sockets_;
    std::auto_ptr< ObjectMessageService >       messageService_;
    std::auto_ptr< Acceptor >                   acceptor_;
    bool                                        stopped_;
    boost::thread                               thread_;
    //@}
};

}

#endif // __ServerNetworker_h_
