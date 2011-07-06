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
#include "tools/ClientNetworker.h"

namespace shield
{
    class ClientListener_ABC;

// =============================================================================
/** @class  Client
    @brief  Client
*/
// Created: MCO 2010-09-30
// =============================================================================
class Client : public tools::ClientNetworker, private Server_ABC, private Client_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Client( const std::string& host, const std::string& from,
                     tools::MessageSender_ABC& sender, ClientListener_ABC& listener );
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

private:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& host );
    virtual void ConnectionFailed( const std::string& host, const std::string& error );
    virtual void ConnectionError( const std::string& host, const std::string& error );

    virtual void Send( const sword::ClientToAar& message );
    virtual void Send( const sword::ClientToAuthentication& message );
    virtual void Send( const sword::ClientToSim& message );
    virtual void Send( const sword::ClientToReplay& message );
    virtual void Send( const sword::ClientToMessenger& message );
    virtual void Send( const sword::AdminToLauncher& message );

    virtual void Send( const MsgsSimToClient::MsgSimToClient& message );
    virtual void Send( const MsgsAuthenticationToClient::MsgAuthenticationToClient& message );
    virtual void Send( const MsgsReplayToClient::MsgReplayToClient& message );
    virtual void Send( const MsgsAarToClient::MsgAarToClient& message );
    virtual void Send( const MsgsMessengerToClient::MsgMessengerToClient& message );
    virtual void Send( const MsgsDispatcherToClient::MsgDispatcherToClient& message );
    virtual void Send( const MsgsLauncherToAdmin::MsgLauncherToAdmin& message );
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void DoSend( const T& message );
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
    //@}
};

}

#endif // shield_Client_h
