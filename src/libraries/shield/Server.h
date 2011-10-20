// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_Server_h
#define shield_Server_h

#include "ClientListener_ABC.h"
#include "Clients.h"
#include "tools/ServerNetworker.h"
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

namespace shield
{
    class Listener_ABC;
    class Client;

// =============================================================================
/** @class  Server
    @brief  Server

    This is the server part of the proxy : it accepts connection from external
    systems and instanciates a Client for each one of them.
*/
// Created: MCO 2010-11-29
// =============================================================================
class Server : private tools::ServerNetworker, private ClientListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Server( unsigned short port, const std::string& host, Listener_ABC& listener, bool encodeStringsInUtf8, unsigned long timeOut = 0 );
    virtual ~Server();
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    //@}

private:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& from );
    virtual void ConnectionFailed( const std::string& from, const std::string& error );
    virtual void ConnectionError( const std::string& from, const std::string& error );
    virtual void ConnectionWarning( const std::string& from, const std::string& warning );

    virtual void Info( const std::string& message );
    virtual void Error( const std::string& from, const std::string& message );
    virtual void Warning( const std::string& from, const std::string& message );
    virtual void Debug( const DebugInfo_ABC& info );
    //@}

    //! @name Helpers
    //@{
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
    typedef std::vector< std::string > T_Errors;
    typedef T_Errors::const_iterator CIT_Errors;
    //@}

private:
    //! @name Member data
    //@{
    Listener_ABC& listener_;
    Clients clients_;
    T_Errors errors_;
    //@}
};

}

#endif // shield_Server_h
