// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MessengerPlugin_h_
#define __MessengerPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "protocol/protocol.h"

namespace MsgsSimToClient
{
    class MsgSimToClient;
}

namespace dispatcher
{
    class Config;
    class ClientPublisher_ABC;
    class LinkResolver_ABC;
    class CompositeRegistrable;
}

namespace tools
{
    class MessageDispatcher_ABC;
    class SessionConfig;
}

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace messenger
{
    class Model;
    class Chat;

// =============================================================================
/** @class  MessengerPlugin
    @brief  MessengerPlugin
*/
// Created: AGE 2008-04-01
// =============================================================================
class MessengerPlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MessengerPlugin( dispatcher::ClientPublisher_ABC& client, tools::MessageDispatcher_ABC&, dispatcher::LinkResolver_ABC& links, const dispatcher::Config& config, dispatcher::CompositeRegistrable& registrables );
    virtual ~MessengerPlugin();
    //@}

    //! @name Network
    //@{
    virtual void Receive                  ( const MsgsSimToClient::MsgSimToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    virtual void Register                 ( dispatcher::Services& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessengerPlugin( const MessengerPlugin& );            //!< Copy constructor
    MessengerPlugin& operator=( const MessengerPlugin& ); //!< Assignment operator
    //@}

    //! @name Network
    //@{
    void OnReceiveClientToMessenger( const std::string&, const MsgsClientToMessenger::MsgClientToMessenger& );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Config& config_ ;
    dispatcher::ClientPublisher_ABC& clients_;
    dispatcher::LinkResolver_ABC& links_;
    Model& model_;
    std::auto_ptr< Chat > chat_;
    //@}
};

}
}

#endif // __MessengerPlugin_h_
