// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_Client_ABC_h
#define shield_Client_ABC_h

#include <boost/noncopyable.hpp>

namespace MsgsAarToClient
{
    class MsgAarToClient;
}

namespace MsgsSimToClient
{
    class MsgSimToClient;
}

namespace MsgsAuthenticationToClient
{
    class MsgAuthenticationToClient;
}

namespace MsgsReplayToClient
{
    class MsgReplayToClient;
}

namespace MsgsMessengerToClient
{
    class MsgMessengerToClient;
}

namespace MsgsDispatcherToClient
{
    class MsgDispatcherToClient;
}

namespace MsgsReplayToClient
{
    class MsgReplayToClient;
}

namespace MsgsLauncherToAdmin
{
    class MsgLauncherToAdmin;
}

namespace shield
{
// =============================================================================
/** @class  Client_ABC
    @brief  Client declaration
*/
// Created: MCO 2010-11-02
// =============================================================================
class Client_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Client_ABC() {}
    virtual ~Client_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( MsgsAarToClient::MsgAarToClient& message ) = 0;
    virtual void Send( MsgsAuthenticationToClient::MsgAuthenticationToClient& message ) = 0;
    virtual void Send( MsgsDispatcherToClient::MsgDispatcherToClient& message ) = 0;
    virtual void Send( MsgsMessengerToClient::MsgMessengerToClient& message ) = 0;
    virtual void Send( MsgsReplayToClient::MsgReplayToClient& message ) = 0;
    virtual void Send( MsgsSimToClient::MsgSimToClient& message ) = 0;
    virtual void Send( MsgsLauncherToAdmin::MsgLauncherToAdmin& message ) = 0;
    //@}
};

}

#endif // shield_Client_ABC_h
