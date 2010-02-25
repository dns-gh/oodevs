// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __CrossbowPlugin_h_
#define __CrossbowPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "dispatcher/NoopPublisher.h"

namespace MsgsSimToClient
{
    class MsgSimToClient;
}

namespace MsgsMessengerToClient
{
    class MsgMessengerToClient;
}

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Model;
    class Config;
    class SimulationPublisher_ABC;
    class LinkResolver_ABC;
    class CompositeRegistrable;
}

namespace tools
{
    class MessageDispatcher_ABC;
    class ClientNetworker;
}

namespace plugins
{
namespace crossbow
{
    class DatabasePublisher;

// =============================================================================
/** @class  CrossbowPlugin
    @brief  CrossbowPlugin
*/
// Created: JCR 2007-08-29
// =============================================================================
class CrossbowPlugin : public dispatcher::Plugin_ABC
                     , public dispatcher::NoopClientPublisher
{

public:
    //! @name Constructors/Destructor
    //@{             
             CrossbowPlugin( const dispatcher::Config& config, xml::xistream& xis, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& links, dispatcher::CompositeRegistrable& registrables );
    virtual ~CrossbowPlugin();
    //@}

    //! @name Operations
    //@{    
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& asnMsg );
    virtual void Send   ( const MsgsMessengerToClient::MsgMessengerToClient& msg ); 
    virtual void Update();
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CrossbowPlugin( const CrossbowPlugin& );            //!< Copy constructor
    CrossbowPlugin& operator=( const CrossbowPlugin& ); //!< Assignement operator
    //@}

    //! @name Copy/Assignement
    //@{
    void OnReceiveMessengerToClient( const std::string& /*link*/, const MsgsMessengerToClient::MsgMessengerToClient& message );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< DatabasePublisher > databasePublisher_;
    std::auto_ptr< tools::ClientNetworker > clientNetworker_;
    std::auto_ptr< dispatcher::Plugin_ABC > messenger_;
    //@}
};

}
}

#endif // __CrossbowPlugin_h_
