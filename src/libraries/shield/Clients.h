// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef shield_Clients_h
#define shield_Clients_h

#include "Server_ABC.h"
#include "Client_ABC.h"
#include "protocol/ClientBroadcaster_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

namespace MsgsClientToAar
{
    class MsgClientToAar;
}

namespace MsgsClientToSim
{
    class MsgClientToSim;
}

namespace MsgsClientToReplay
{
    class MsgClientToReplay;
}

namespace MsgsClientToAuthentication
{
    class MsgClientToAuthentication;
}

namespace MsgsClientToMessenger
{
    class MsgClientToMessenger;
}

namespace MsgsAdminToLauncher
{
    class MsgAdminToLauncher;
}

namespace tools
{
    class MessageSender_ABC;
    class MessageDispatcher_ABC;
}

namespace shield
{
    class ClientHandler_ABC;
    class ClientListener_ABC;
    class Listener_ABC;
    class Client;

// =============================================================================
/** @class  Clients
    @brief  Clients
*/
// Created: MCO 2011-09-20
// =============================================================================
class Clients : private dispatcher::ClientBroadcaster_ABC
              , private Server_ABC, private Client_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Clients( tools::MessageSender_ABC& sender, tools::MessageDispatcher_ABC& dispatcher,
                      ClientHandler_ABC& handler, ClientListener_ABC& listener,
                      bool encodeStringsInUtf8 );
    virtual ~Clients();
    //@}

    //! @name Operations
    //@{
    void Add( const std::string& from );
    void Remove( const std::string& from );

    void ReceiveClientToAar           ( const std::string& from, const MsgsClientToAar::MsgClientToAar& msg );
    void ReceiveClientToAuthentication( const std::string& from, const MsgsClientToAuthentication::MsgClientToAuthentication& msg );
    void ReceiveClientToMessenger     ( const std::string& from, const MsgsClientToMessenger::MsgClientToMessenger& msg );
    void ReceiveClientToReplay        ( const std::string& from, const MsgsClientToReplay::MsgClientToReplay& msg );
    void ReceiveClientToSim           ( const std::string& from, const MsgsClientToSim::MsgClientToSim& msg );
    void ReceiveAdminToLauncher       ( const std::string& from, const MsgsAdminToLauncher::MsgAdminToLauncher& msg );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< Client > > T_Clients;
    typedef T_Clients::const_iterator                          CIT_Clients;
    //@}

private:
    //! @name Operations
    //@{
    virtual void Activate( const std::string& link );
    virtual void Deactivate( const std::string& link );

    virtual void Broadcast( const sword::SimToClient& message );

    virtual void Send( sword::ClientToSim& message );
    virtual void Send( sword::ClientToAuthentication& message );
    virtual void Send( sword::ClientToReplay& message );
    virtual void Send( sword::ClientToAar& message );
    virtual void Send( sword::ClientToMessenger& message );
    virtual void Send( sword::AdminToLauncher& message );

    virtual void Send( MsgsAarToClient::MsgAarToClient& message );
    virtual void Send( MsgsAuthenticationToClient::MsgAuthenticationToClient& message );
    virtual void Send( MsgsDispatcherToClient::MsgDispatcherToClient& message );
    virtual void Send( MsgsMessengerToClient::MsgMessengerToClient& message );
    virtual void Send( MsgsReplayToClient::MsgReplayToClient& message );
    virtual void Send( MsgsSimToClient::MsgSimToClient& message );
    virtual void Send( MsgsLauncherToAdmin::MsgLauncherToAdmin& message );
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageSender_ABC& sender_;
    tools::MessageDispatcher_ABC& dispatcher_;
    ClientHandler_ABC& handler_;
    ClientListener_ABC& listener_;
    bool utf8StringEncoding_;
    T_Clients clients_;
    T_Clients actives_;
    //@}
};

}

#endif // shield_Clients_h
