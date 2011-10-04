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
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/Agent.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Karma.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AgentController constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentController::AgentController( xml::xisubstream xis, dispatcher::Model_ABC& model, const rpr::EntityTypeResolver_ABC& aggregatesResolver, const rpr::EntityTypeResolver_ABC& componentTypeResolver, const ComponentTypes_ABC& componentTypes )
    : model_                ( model )
    , aggregatesResolver_   ( aggregatesResolver )
    , componentTypeResolver_( componentTypeResolver )
    , componentTypes_       ( componentTypes )
{
    xis >> xml::start( "surfaceVessels" )
            >> xml::list( "unit", *this, &AgentController::ReadSurfaceVessel );
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
    if( !boost::algorithm::starts_with( agent.GetName().toStdString(), "HLA_" ) ) // $$$$ _RC_ SLI 2011-09-22: refactor this...
    {
        agents_.push_back( T_Agent( new AgentProxy( agent, componentTypes_, componentTypeResolver_ ) ) );
        const kernel::AgentType& agentType = agent.GetType();
        const std::string typeName = agentType.GetName();
        const rpr::EntityType entityType = aggregatesResolver_.Find( typeName );
        const rpr::ForceIdentifier forceIdentifier = GetForce( agent );
        const bool isSurfaceVessel = surfaceVessels_.find( typeName ) != surfaceVessels_.end();
        for( CIT_Listeners it = listeners_.begin(); it != listeners_.end(); ++it )
            if( isSurfaceVessel )
                (*it)->SurfaceVesselCreated( *agents_.back(), agent.GetId(), agent.GetName().toStdString(), forceIdentifier, entityType );
            else
                (*it)->AggregateCreated( *agents_.back(), agent.GetId(), agent.GetName().toStdString(), forceIdentifier, entityType );
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
// Name: AgentController::ReadSurfaceVessel
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void AgentController::ReadSurfaceVessel( xml::xistream& xis )
{
    std::string unit;
    xis >> unit;
    surfaceVessels_.insert( unit );
}
