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
    class BufferedMessageCallback;
    class BufferedConnectionCallback;
    class SocketManager;
    class Connector;
    class WaitEvent;

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
    virtual unsigned long GetNbMessagesReceived() const;
    virtual unsigned long GetNbMessagesSent() const; // $$$$ MCO : make a proxy on MessageSender_ABC instead
    virtual bool HasAnsweredSinceLastTick( const std::string& endpoint );
    // Return the total size of received messages in bytes.
    uint64_t GetReceivedAmount() const;
    uint64_t GetSentAmount() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& local, const std::string& remote );
    virtual void ConnectionFailed( const std::string& address, const std::string& error );
    virtual void ConnectionError( const std::string& address, const std::string& error ) = 0;
    virtual void ConnectionWarning( const std::string& address, const std::string& warning ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ClientNetworker( const ClientNetworker& );            //!< Copy constructor
    ClientNetworker& operator=( const ClientNetworker& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
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
    boost::shared_ptr< BufferedConnectionCallback > connectionBuffer_;
    boost::shared_ptr< BufferedMessageCallback >    messageBuffer_;
    std::unique_ptr< SocketManager >                sockets_;
    std::unique_ptr< ObjectMessageService >         messageService_;
    std::unique_ptr< Connector >                    connector_;
    std::unique_ptr< WaitEvent >                    quit_;
    std::string                                     host_;
    bool                                            retry_;
    boost::thread                                   thread_;
    unsigned long                                   inMessages_;
    uint64_t                                        inBytes_;
   //@}
};

}

#endif // __ClientNetworker_h_
