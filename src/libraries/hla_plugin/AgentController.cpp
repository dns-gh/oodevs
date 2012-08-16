// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AgentController.h"
#include "AgentListener_ABC.h"
#include "AgentProxy.h"
#include "AgentAdapter.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/Agent.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/Karma.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include "tic/PlatformDelegateFactory_ABC.h"
#include "tic/Platform_ABC.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <limits>
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AgentController constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentController::AgentController( dispatcher::Model_ABC& model, const rpr::EntityTypeResolver_ABC& aggregatesResolver,
                                  const rpr::EntityTypeResolver_ABC& componentTypeResolver, const ComponentTypes_ABC& componentTypes,
                                  tic::PlatformDelegateFactory_ABC& factory, const kernel::CoordinateConverter_ABC& converter,
                                  bool sendPlatforms )
    : model_                 ( model )
    , aggregatesResolver_    ( aggregatesResolver )
    , componentTypeResolver_ ( componentTypeResolver )
    , componentTypes_        ( componentTypes )
    , factory_               ( factory )
    , converter_             ( converter )
    , doDisaggregation_      ( sendPlatforms )
{
    model_.RegisterFactory( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentController destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentController::~AgentController()
{
    model_.UnregisterFactory( *this );
}

namespace
{
    rpr::ForceIdentifier GetForce( const dispatcher::Agent_ABC& agent )
    {
        const kernel::Karma& karma = agent.GetSuperior().GetTeam().GetKarma();
        if( karma == kernel::Karma::friend_ )
            return rpr::Friendly;
        if( karma == kernel::Karma::enemy_ )
            return rpr::Opposing;
        if( karma == kernel::Karma::neutral_ )
            return rpr::Neutral;
        return rpr::Other;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentController::Visit
// Created: SLI 2011-09-28
// -----------------------------------------------------------------------------
void AgentController::Visit( dispatcher::Model_ABC& model )
{
    for( tools::Iterator< const dispatcher::Agent_ABC& > it = model.Agents().CreateIterator(); it.HasMoreElements(); )
    {
        dispatcher::Agent_ABC& agent = const_cast< dispatcher::Agent_ABC& >( it.NextElement() ); // $$$$ _RC_ SLI 2011-09-28: erk...
        CreateAgent( agent );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentController::Create
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void AgentController::Create( dispatcher::Agent& entity )
{
    CreateAgent( entity );
}

// -----------------------------------------------------------------------------
// Name: AgentController::CreateAgent
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void AgentController::CreateAgent( dispatcher::Agent_ABC& agent )
{
    if( !boost::algorithm::starts_with( agent.GetName().toAscii().constData(), "HLA_" ) ) // $$$$ _RC_ SLI 2011-09-22: refactor this...
    {
        T_Agents::iterator itAgent( agents_.insert( T_Agents::value_type( agent.GetId(), T_Agent( new AgentProxy( agent, componentTypes_, componentTypeResolver_, doDisaggregation_ ) ) ) ).first );
        const kernel::AgentType& agentType = agent.GetType();
        const std::string typeName = agentType.GetName();
        const rpr::EntityType entityType = aggregatesResolver_.Find( typeName );
        const rpr::ForceIdentifier forceIdentifier = GetForce( agent );
        for( CIT_Listeners it = listeners_.begin(); it != listeners_.end(); ++it )
            (*it)->AggregateCreated( *(itAgent->second), agent.GetId(), agent.GetName().toAscii().constData(), forceIdentifier, entityType, agentType.GetSymbol() );
        if( doDisaggregation_ )
            adapters_.push_back( T_AgentAdapter( new AgentAdapter( factory_, converter_, agent,
                    AgentAdapter::T_NotificationCallback( boost::bind( &AgentController::NotifyPlatformCreation, boost::ref( *this ), _1, _2, _3, _4 ) ) ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentController::Register
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
void AgentController::Register( AgentListener_ABC& listener )
{
    listeners_.push_back( &listener );
}

// -----------------------------------------------------------------------------
// Name: AgentController::Unregister
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
void AgentController::Unregister( AgentListener_ABC& listener )
{
    listeners_.erase( std::remove( listeners_.begin(), listeners_.end(), &listener ), listeners_.end() );
}

// -----------------------------------------------------------------------------
// Name: AgentController::NotifyPlatformCreation
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void AgentController::NotifyPlatformCreation( Agent_ABC& agent, dispatcher::Agent_ABC& parent, const tic::Platform_ABC& platform, int childIndex )
{
    static unsigned int identifier_factory = std::numeric_limits< short >::max();
    const std::string symbol( parent.GetType().GetSymbol() ); // FIXME
    const std::string typeName = platform.GetType().GetName();
    const rpr::EntityType entityType = componentTypeResolver_.Find( typeName );
    const rpr::ForceIdentifier forceIdentifier = GetForce( parent );
    unsigned int identifier = --identifier_factory;  // FIXME
    const std::string name( std::string( parent.GetName().toAscii().constData() ) + "_" + typeName + " " + boost::lexical_cast< std::string >( childIndex ) );

    for( CIT_Listeners it = listeners_.begin(); it != listeners_.end(); ++it )
        (*it)->PlatformCreated( agent, identifier, name, forceIdentifier, entityType, symbol );

    T_Agents::const_iterator itAgent( agents_.find( parent.GetId() ) );
    if( agents_.end() !=  itAgent )
    {
        AgentProxy& proxy( *itAgent->second );
        proxy.PlatformAdded( name, identifier );
    }
}
