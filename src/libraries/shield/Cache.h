// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef shield_Cache_h
#define shield_Cache_h

#include "Server_ABC.h"
#include "Client_ABC.h"
#include "tools/Message.h"
#include "protocol/ClientPublisher_ABC.h"

namespace tools
{
    class MessageSender_ABC;
}

namespace shield
{
    class Model_ABC;

// =============================================================================
/** @class  Cache
    @brief  Cache
*/
// Created: MCO 2011-11-09
// =============================================================================
class Cache : private dispatcher::ClientPublisher_ABC
            , private Server_ABC, private Client_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Cache( tools::MessageSender_ABC& sender, const Model_ABC& model, bool utf8StringEncoding );
    virtual ~Cache();
    //@}

    //! @name Operations
    //@{
    void Send( const std::string& link );
    void Clear();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Send( const sword::SimToClient&            message );
    virtual void Send( const sword::AuthenticationToClient& message );
    virtual void Send( const sword::ReplayToClient&         message );
    virtual void Send( const sword::AarToClient&            message );
    virtual void Send( const sword::MessengerToClient&      message );
    virtual void Send( const sword::DispatcherToClient&     message );

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
    //! @name Types
    //@{
    typedef std::vector< tools::Message > T_Messages;
    typedef T_Messages::iterator  IT_Messages;
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageSender_ABC& sender_;
    const Model_ABC& model_;
    bool utf8StringEncoding_;
    T_Messages messages_;
    //@}
};

}

#endif // shield_Cache_h
