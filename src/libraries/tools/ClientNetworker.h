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

#include "ConnectionCallback_ABC.h"
#include "MessageSender_ABC.h"
#include "MessageDispatcher_ABC.h"
#pragma warning( push )
#pragma warning( disable : 4512 )
#include <boost/thread.hpp>
#pragma warning( pop )
#include <string>
#include <memory>

namespace boost { 
    namespace asio {
        class io_service;
    }
}

namespace tools
{
    class ObjectMessageService;
    class BufferedMessageCallback;
    class BufferedConnectionCallback;
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
                     , private ConnectionCallback_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ClientNetworker( const std::string& host = "", bool retry = false );
    virtual ~ClientNetworker();
    //@}

    //! @name Operations
    //@{
    void Update();

    void Connect   ( const std::string& host, bool retry );
    void Disconnect();

    using MessageSender_ABC::Send;
    using MessageDispatcher_ABC::RegisterMessage;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );
    //@}

private:
    //! @name Helpers
    //@{
    void Run();
    void Stop();
    virtual void Send( const std::string& endpoint, unsigned long tag, const Message& message );
    virtual void Register( unsigned long id, std::auto_ptr< ObjectMessageCallback_ABC > callback );
    virtual ObjectMessageCallback_ABC* Retrieve( unsigned long id );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ClientNetworker( const ClientNetworker& );            //!< Copy constructor
    ClientNetworker& operator=( const ClientNetworker& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< boost::asio::io_service >    service_;
    std::auto_ptr< BufferedConnectionCallback > connectionBuffer_;
    std::auto_ptr< BufferedMessageCallback >    messageBuffer_;
    std::auto_ptr< SocketManager >              sockets_;
    std::auto_ptr< ObjectMessageService >       messageService_;
    std::auto_ptr< Connector >                  connector_;
    std::string                                 host_; 
    bool                                        retry_;
    volatile bool                               stopped_;
    boost::thread                               thread_;
    //@}
    
};

}

#endif // __ClientNetworker_h_
