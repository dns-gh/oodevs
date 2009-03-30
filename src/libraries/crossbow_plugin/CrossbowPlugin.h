// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossbowPlugin_h_
#define __CrossbowPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "dispatcher/NoopPublisher.h"
#include "game_asn/Messenger.h"

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
    virtual void Receive( const ASN1T_MsgsSimToClient& asnMsg );
    virtual void Send   ( const ASN1T_MsgsMessengerToClient& msg ); 
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
