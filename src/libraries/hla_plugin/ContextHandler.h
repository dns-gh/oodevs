// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ContextHandler_h
#define plugins_hla_ContextHandler_h

#include "ContextHandler_ABC.h"
#include "ResponseObserver_ABC.h"
#include "tools/MessageObserver.h"
#include "protocol/SimulationSenders.h"
#include <map>
#include <vector>
#include <boost/foreach.hpp>

namespace tools
{
    template< typename Category > class MessageController_ABC;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace hla
{
    class ContextFactory_ABC;

// =============================================================================
/** @class  ContextHandler
    @brief  Context handler
*/
// Created: SLI 2011-09-09
// =============================================================================
template< typename MessageResponse >
class ContextHandler : public ContextHandler_ABC< MessageResponse >
                     , public tools::MessageObserver< MessageResponse >
                     , private tools::MessageObserver< sword::UnitMagicActionAck >
{
public:
    //! @name Constructors/Destructor
    //@{
             ContextHandler( const std::string& errorMessage, tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                             const ContextFactory_ABC& contextFactory, dispatcher::SimulationPublisher_ABC& publisher )
                 : errorMessage_  ( errorMessage )
                 , contextFactory_( contextFactory )
                 , publisher_     ( publisher )
             {
                 CONNECT( messageController, *this, unit_magic_action_ack );
             }
    virtual ~ContextHandler() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( ResponseObserver_ABC< MessageResponse >& observer )
    {
        observers_.push_back( &observer );
    }
    virtual void Unregister( ResponseObserver_ABC< MessageResponse >& observer )
    {
        observers_.erase( std::remove( observers_.begin(), observers_.end(), &observer ), observers_.end() );
    }
    virtual void Send( simulation::UnitMagicAction& message, const std::string& identifier )
    {
        const int context = contextFactory_.Create();
        contexts_[ context ] = identifier;
        message.Send( publisher_, context );
    }
    virtual void Send( simulation::ObjectMagicAction& message, const std::string& identifier )
    {
        const int context = contextFactory_.Create();
        contexts_[ context ] = identifier;
        message.Send( publisher_, context );
    }
    virtual void Notify( const MessageResponse& message, int context )
    {
        if( contexts_.find( context ) != contexts_.end() )
        {
            BOOST_FOREACH( ResponseObserver_ABC< MessageResponse >* observer, observers_ )
                observer->Notify( message, contexts_[ context ] );
            contexts_.erase( context );
        }
    }
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::UnitMagicActionAck& message, int context )
    {
        if( message.error_code() != sword::UnitActionAck::no_error && contexts_.find( context ) != contexts_.end() )
            throw MASA_EXCEPTION( "Error while creating distant " + errorMessage_ + " '" + contexts_[ context ] + "'" );
    }
    //@}

private:
    //! @name Member data
    //@{
    const std::string errorMessage_;
    std::vector< ResponseObserver_ABC< MessageResponse >* > observers_;
    const ContextFactory_ABC& contextFactory_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    std::map< int, std::string > contexts_;
    //@}
};

}
}

#endif // plugins_hla_ContextHandler_h
