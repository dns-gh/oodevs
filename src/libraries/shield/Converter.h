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
             Converter( const std::string& from, Server_ABC& server, Client_ABC& client );
    virtual ~Converter();
    //@}

    //! @name Operations
    //@{
    void ReceiveAarToClient           ( const std::string& from, const sword::AarToClient& msg );
    void ReceiveAuthenticationToClient( const std::string& from, const sword::AuthenticationToClient& msg );
    void ReceiveDispatcherToClient    ( const std::string& from, const sword::DispatcherToClient& msg );
    void ReceiveMessengerToClient     ( const std::string& from, const sword::MessengerToClient& msg );
    void ReceiveReplayToClient        ( const std::string& from, const sword::ReplayToClient& msg );
    void ReceiveSimToClient           ( const std::string& from, const sword::SimToClient& msg );
    void ReceiveLauncherToAdmin       ( const std::string& from, const sword::LauncherToAdmin& msg );

    void ReceiveClientToAar           ( const std::string& from, const MsgsClientToAar::MsgClientToAar& msg );
    void ReceiveClientToAuthentication( const std::string& from, const MsgsClientToAuthentication::MsgClientToAuthentication& msg );
    void ReceiveClientToMessenger     ( const std::string& from, const MsgsClientToMessenger::MsgClientToMessenger& msg );
    void ReceiveClientToReplay        ( const std::string& from, const MsgsClientToReplay::MsgClientToReplay& msg );
    void ReceiveClientToSim           ( const std::string& from, const MsgsClientToSim::MsgClientToSim& msg );
    void ReceiveAdminToLauncher       ( const std::string& from, const MsgsAdminToLauncher::MsgAdminToLauncher& msg );
    //@}

private:
    //! @name Member data
    //@{
    const std::string from_;
    Server_ABC& server_;
    Client_ABC& client_;
    //@}
};

}

#endif // shield_Converter_h
