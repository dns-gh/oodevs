// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientNetworker_h_
#define __ClientNetworker_h_

#include "SocketEventCallback_ABC.h"
#include "MessageSender_ABC.h"
#include "MessageDispatcher_ABC.h"
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
    class BufferedSocketEventCallback;
    class SocketManager;
    class Connector;

// =============================================================================
/** @class  ClientNetworker
    @brief  ClientNetworker
*/
// Created: NLD 2006-09-19
// =============================================================================
class ClientNetworker: public MessageDispatcher_ABC
                     , public MessageSender_ABC
                     , private SocketEventCallback_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ClientNetworker( const std::string& host = "", bool retry = false, unsigned long timeOut = 10000 );
    virtual ~ClientNetworker();
    //@}

    //! @name Operations
    //@{
    void Update();

    void Connect( const std::string& host, bool retry = false );
    void Disconnect();

    using MessageSender_ABC::Send;
    using MessageDispatcher_ABC::RegisterMessage;

    virtual void Send( const std::string& endpoint, unsigned long tag, Message& message );
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetNbMessagesReceived() const; // $$$$ MCO : make a proxy on MessageDispatcher_ABC instead
    virtual unsigned long GetNbMessagesSent() const; // $$$$ MCO : make a proxy on MessageSender_ABC instead
    virtual bool HasAnsweredSinceLastTick( const std::string& endpoint );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );
    virtual void ConnectionWarning  ( const std::string& address, const std::string& warning );
    virtual void OnMessage          ( const std::string& endpoint, Message& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ClientNetworker( const ClientNetworker& );            //!< Copy constructor
    ClientNetworker& operator=( const ClientNetworker& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
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
    std::auto_ptr< boost::asio::io_service >         service_;
    boost::shared_ptr< BufferedSocketEventCallback > eventsBuffer_;
    std::auto_ptr< SocketManager >                   sockets_;
    std::auto_ptr< ObjectMessageService >            messageService_;
    std::auto_ptr< Connector >                       connector_;
    std::string                                      host_;
    bool                                             retry_;
    bool                                             stopped_;
    boost::thread                                    thread_;
   //@}
};

}

#endif // __ClientNetworker_h_
