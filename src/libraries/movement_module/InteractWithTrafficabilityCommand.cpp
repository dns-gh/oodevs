// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "InteractWithTrafficabilityCommand.h"
#include "wrapper/View.h"
#include "wrapper/Event.h"
#include "wrapper/Hook.h"
#include "MT_Tools/MT_Vector2D.h"
#include <boost/foreach.hpp>

using namespace sword;
using namespace sword::movement;

DECLARE_HOOK( GetAgentListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )

// -----------------------------------------------------------------------------
// Name: InteractWithTrafficabilityCommand constructor
// Created: MMC 2013-04-16
// -----------------------------------------------------------------------------
InteractWithTrafficabilityCommand::InteractWithTrafficabilityCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractWithTrafficabilityCommand::Destroy
// Created: MMC 2012-10-26
// -----------------------------------------------------------------------------
void InteractWithTrafficabilityCommand::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}

namespace
{
    void AddAgent( const SWORD_Model* agent, void* userData )
    {
        static_cast< std::vector< wrapper::View >* >( userData )->push_back( agent );
    }
}

// -----------------------------------------------------------------------------
// Name: InteractWithTrafficabilityCommand::Execute
// Created: MMC 2013-04-16
// -----------------------------------------------------------------------------
void InteractWithTrafficabilityCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const std::size_t identifier = parameters[ "identifier" ];
    const wrapper::View& entity = model[ "entities" ][ identifier ];
    if( !entity[ "movement/can-be-traffic-impacted" ] )
        return;
    const MT_Vector2D position( entity[ "movement/position/x" ], entity[ "movement/position/y" ] );
    const float radius = entity[ "perceptions/max-agent-perception-distance" ];
    std::vector< wrapper::View > agents;
    GET_HOOK( GetAgentListWithinCircle )( model, position, radius, &AddAgent, &agents );
    if( agents.empty() )
        return;
    wrapper::Event event( "trafficability interaction" );
    event[ "entity" ] = identifier;
    BOOST_FOREACH( const wrapper::View& agent, agents )
        event[ "interactions" ].AddElement()[ "data" ] = agent[ "data" ];
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: InteractWithTrafficabilityCommand::ExecutePaused
// Created: MMC 2013-04-16
// -----------------------------------------------------------------------------
void InteractWithTrafficabilityCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
