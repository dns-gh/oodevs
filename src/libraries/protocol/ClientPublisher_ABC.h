// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientPublisher_ABC_h_
#define __ClientPublisher_ABC_h_



////using namespace Common;
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

namespace MsgsPluginToClient
{
    class MsgPluginToClient;
}

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


namespace dispatcher
{

// =============================================================================
/** @class  ClientPublisher_ABC
    @brief  ClientPublisher_ABC
*/
// Created: AGE 2007-08-24
// =============================================================================
class ClientPublisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ClientPublisher_ABC() {};
    virtual ~ClientPublisher_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Send( const MsgsSimToClient::MsgSimToClient&            msg ) = 0;
    virtual void Send( const MsgsAuthenticationToClient::MsgAuthenticationToClient& msg ) = 0;
    virtual void Send( const MsgsReplayToClient::MsgReplayToClient&         msg ) = 0;
    virtual void Send( const MsgsAarToClient::MsgAarToClient&            msg ) = 0;
    virtual void Send( const MsgsMessengerToClient::MsgMessengerToClient&      msg ) = 0;
    virtual void Send( const MsgsDispatcherToClient::MsgDispatcherToClient&     msg ) = 0;
    virtual void Send( const MsgsPluginToClient::MsgPluginToClient&         msg ) = 0;
    
    
    
//    virtual void Send( const MsgClientToAar&         msg ) = 0;
//    virtual void Send( const MsgClientToSim&         msg ) = 0;
//    virtual void Send( const MsgClientToReplay&      msg ) = 0;
//    virtual void Send( const MsgClientToAuthentication& msg) = 0;

    //@}

    //! @name Accessors
    //@{
    virtual std::string GetEndpoint() const = 0;
    //@}
};

}

#endif // __ClientPublisher_ABC_h_
