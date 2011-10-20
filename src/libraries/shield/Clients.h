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

#include "tools/asio.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )
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
}

namespace shield
{
    class ClientListener_ABC;
    class Listener_ABC;
    class Client;

// =============================================================================
/** @class  Clients
    @brief  Clients
*/
// Created: MCO 2011-09-20
// =============================================================================
class Clients : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Clients( const std::string& host, tools::MessageSender_ABC& sender, ClientListener_ABC& listener,
                      bool encodeStringsInUtf8, unsigned long timeOut );
    virtual ~Clients();
    //@}

    //! @name Operations
    //@{
    void Add( const std::string& from );
    void Remove( const std::string& from );

    void Update();

    void ReceiveClientToAar           ( const std::string& from, const MsgsClientToAar::MsgClientToAar& msg );
    void ReceiveClientToAuthentication( const std::string& from, const MsgsClientToAuthentication::MsgClientToAuthentication& msg );
    void ReceiveClientToMessenger     ( const std::string& from, const MsgsClientToMessenger::MsgClientToMessenger& msg );
    void ReceiveClientToReplay        ( const std::string& from, const MsgsClientToReplay::MsgClientToReplay& msg );
    void ReceiveClientToSim           ( const std::string& from, const MsgsClientToSim::MsgClientToSim& msg );
    void ReceiveAdminToLauncher       ( const std::string& from, const MsgsAdminToLauncher::MsgAdminToLauncher& msg );
    //@}

private:
    //! @name Helpers
    //@{
    void Run();
    void Stop();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< Client > > T_Clients;
    typedef T_Clients::const_iterator                          CIT_Clients;
    //@}

private:
    //! @name Member data
    //@{
    const std::string host_;
    tools::MessageSender_ABC& sender_;
    ClientListener_ABC& listener_;
    bool utf8StringEncoding_;
    const unsigned long timeOut_;
    boost::asio::io_service service_;
    T_Clients clients_;
    bool stopped_;
    boost::thread thread_;
    //@}
};

}

#endif // shield_Clients_h
