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
#include "tools/MessageSender_ABC.h"
#include "tools/ObjectMessageService.h"
#include <string>
#include <memory>

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace shield
{
    class ClientHandler_ABC;
    class ClientListener_ABC;

// =============================================================================
/** @class  Client
    @brief  Client

    This is the client part of the proxy : it connects to the 'real' dispatcher.
*/
// Created: MCO 2010-09-30
// =============================================================================
class Client : private Server_ABC, private Client_ABC
             , public tools::MessageSender_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Client( const std::string& from, tools::MessageSender_ABC& sender, tools::MessageDispatcher_ABC& dispatcher,
                     ClientHandler_ABC& handler, ClientListener_ABC& listener, bool encodeStringsInUtf8 );
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

    virtual void Send( const std::string& link, unsigned long tag, const google::protobuf::Message& m );
    virtual void Send( const std::string& link, unsigned long tag, const tools::Message& message );
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetNbMessagesSent() const;
    //@}

private:
    //! @name Operations
    //@{
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

    //! @name Helpers
    //@{
    template< typename T >
    void DoSend( T& message );

    template< typename T >
    void Notify( const T& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string from_;
    tools::MessageSender_ABC& sender_;
    tools::MessageDispatcher_ABC& dispatcher_;
    ClientHandler_ABC& handler_;
    ClientListener_ABC& listener_;
    Converter converter_;
    tools::ObjectMessageService service_;
    bool encodeStringsInUtf8_; // $$$$ MCO : make a proxy !
    //@}
};

}

#endif // shield_Client_h
