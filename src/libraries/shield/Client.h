// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_Client_h
#define shield_Client_h

#include "Converter.h"
#include "Server_ABC.h"
#include "Client_ABC.h"
#include "tools/ConnectionCallback_ABC.h"
#include "tools/MessageSender_ABC.h"
#include "tools/MessageDispatcher_ABC.h"
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
}

namespace shield
{
    class ClientListener_ABC;

// =============================================================================
/** @class  Client
    @brief  Client

    This is the client part of the proxy : it connects to the 'real' dispatcher.
*/
// Created: MCO 2010-09-30
// =============================================================================
class Client : public tools::MessageDispatcher_ABC , public tools::MessageSender_ABC
             , private tools::ConnectionCallback_ABC, private Server_ABC, private Client_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Client( boost::asio::io_service& service, const std::string& from, tools::MessageSender_ABC& sender,
                     ClientListener_ABC& listener, bool encodeStringsInUtf8, unsigned long timeOut );
    virtual ~Client();
    //@}

    //! @name Operations
    //@{
    void ReceiveClientToAar           ( const MsgsClientToAar::MsgClientToAar& msg );
    void ReceiveClientToAuthentication( const MsgsClientToAuthentication::MsgClientToAuthentication& msg );
    void ReceiveClientToMessenger     ( const MsgsClientToMessenger::MsgClientToMessenger& msg );
    void ReceiveClientToReplay        ( const MsgsClientToReplay::MsgClientToReplay& msg );
    void ReceiveClientToSim           ( const MsgsClientToSim::MsgClientToSim& msg );
    void ReceiveAdminToLauncher       ( const MsgsAdminToLauncher::MsgAdminToLauncher& msg );
    //@}

    //! @name Operations
    //@{
    void Update();

    void Connect( const std::string& host );
    void Disconnect();

    using MessageSender_ABC::Send;
    using MessageDispatcher_ABC::RegisterMessage;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetNbMessagesReceived() const; // $$$$ MCO : make a proxy on MessageDispatcher_ABC instead
    virtual unsigned long GetNbMessagesSent() const; // $$$$ MCO : make a proxy on MessageSender_ABC instead
    //@}

private:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& host );
    virtual void ConnectionFailed( const std::string& host, const std::string& error );

    virtual void Send( sword::ClientToAar& message );
    virtual void Send( sword::ClientToAuthentication& message );
    virtual void Send( sword::ClientToSim& message );
    virtual void Send( sword::ClientToReplay& message );
    virtual void Send( sword::ClientToMessenger& message );
    virtual void Send( sword::AdminToLauncher& message );

    virtual void Send( MsgsSimToClient::MsgSimToClient& message );
    virtual void Send( MsgsAuthenticationToClient::MsgAuthenticationToClient& message );
    virtual void Send( MsgsReplayToClient::MsgReplayToClient& message );
    virtual void Send( MsgsAarToClient::MsgAarToClient& message );
    virtual void Send( MsgsMessengerToClient::MsgMessengerToClient& message );
    virtual void Send( MsgsDispatcherToClient::MsgDispatcherToClient& message );
    virtual void Send( MsgsLauncherToAdmin::MsgLauncherToAdmin& message );
    //@}

    //! @name Operations
    //@{
    virtual void Send( const std::string& endpoint, unsigned long tag, const tools::Message& message );
    virtual void Register( unsigned long id, std::auto_ptr< tools::ObjectMessageCallback_ABC > callback );
    virtual tools::ObjectMessageCallback_ABC* Retrieve( unsigned long id );
    //@}

    //! @name Helpers
    //@{
    void ConnectionError( const std::string& host, const std::string& error );
    void ConnectionWarning( const std::string& host, const std::string& warning );

    template< typename T >
    void DoSend( T& message );

    template< typename T >
    void Notify( const T& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    typedef std::vector< boost::function< void() > > T_Callbacks;
    //@}

private:
    //! @name Member data
    //@{
    std::string host_;
    const std::string from_;
    T_Callbacks callbacks_;
    tools::MessageSender_ABC& sender_;
    ClientListener_ABC& listener_;
    Converter converter_;
    bool encodeStringsInUtf8_;
    boost::shared_ptr< tools::BufferedConnectionCallback > connectionBuffer_;
    boost::shared_ptr< tools::BufferedMessageCallback >    messageBuffer_;
    std::auto_ptr< tools::SocketManager >                  sockets_;
    std::auto_ptr< tools::ObjectMessageService >           messageService_;
    std::auto_ptr< tools::Connector >                      connector_;
    //@}
};

}

#endif // shield_Client_h
