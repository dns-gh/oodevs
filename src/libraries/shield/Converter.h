// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_Converter_h
#define shield_Converter_h

#include "protocol/Protocol.h"
#include <boost/noncopyable.hpp>

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

namespace shield
{
    class Client_ABC;
    class Server_ABC;
    class ClientListener_ABC;

// =============================================================================
/** @class  Converter
    @brief  Converter
*/
// Created: MCO 2010-10-27
// =============================================================================
class Converter : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Converter( Server_ABC& server, Client_ABC& client, ClientListener_ABC& listener );
    virtual ~Converter();
    //@}

    //! @name Operations
    //@{
    void ReceiveAarToClient           ( const sword::AarToClient& msg );
    void ReceiveAuthenticationToClient( const sword::AuthenticationToClient& msg );
    void ReceiveDispatcherToClient    ( const sword::DispatcherToClient& msg );
    void ReceiveMessengerToClient     ( const sword::MessengerToClient& msg );
    void ReceiveReplayToClient        ( const sword::ReplayToClient& msg );
    void ReceiveSimToClient           ( const sword::SimToClient& msg );
    void ReceiveLauncherToAdmin       ( const sword::LauncherToAdmin& msg );

    void ReceiveClientToAar           ( const MsgsClientToAar::MsgClientToAar& msg );
    void ReceiveClientToAuthentication( const MsgsClientToAuthentication::MsgClientToAuthentication& msg );
    void ReceiveClientToMessenger     ( const MsgsClientToMessenger::MsgClientToMessenger& msg );
    void ReceiveClientToReplay        ( const MsgsClientToReplay::MsgClientToReplay& msg );
    void ReceiveClientToSim           ( const MsgsClientToSim::MsgClientToSim& msg );
    void ReceiveAdminToLauncher       ( const MsgsAdminToLauncher::MsgAdminToLauncher& msg );
    //@}

private:
    //! @name Member data
    //@{
    const std::string from_;
    Server_ABC& server_;
    Client_ABC& client_;
    ClientListener_ABC& listener_;
    //@}
};

}

#endif // shield_Converter_h
