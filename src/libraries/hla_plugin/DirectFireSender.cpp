// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "DirectFireSender.h"
#include "Omt13String.h"
#include "Interactions.h"
#include "InteractionSender_ABC.h"
#include "RemoteAgentResolver_ABC.h"
#include "RemoteAgentSubject_ABC.h"
#include "protocol/Simulation.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DirectFireSender constructor
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
DirectFireSender::DirectFireSender( InteractionSender_ABC< interactions::MunitionDetonation >& interactionSender,
                                    const RemoteAgentResolver_ABC& resolver, RemoteAgentSubject_ABC& remoteAgentSubject,
                                    tools::MessageController_ABC< sword::SimToClient_Content >& controller, const std::string& federateName )
    : interactionSender_ ( interactionSender )
    , resolver_          ( resolver )
    , remoteAgentSubject_( remoteAgentSubject )
    , federateName_      ( federateName )
{
    CONNECT( controller, *this, start_unit_fire );
    CONNECT( controller, *this, stop_unit_fire );
    remoteAgentSubject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender destructor
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
DirectFireSender::~DirectFireSender()
{
    remoteAgentSubject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Notify
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::Notify( const sword::StartUnitFire& message, int /*context*/ )
{
    if( message.type() == sword::StartUnitFire::direct )
        fires_[ message.fire().id() ] = message;
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Notify
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::Notify( const sword::StopUnitFire& message, int /*context*/ )
{
    const unsigned int fireIdentifier = message.fire().id();
    if( fires_.find( fireIdentifier ) == fires_.end() )
        return;
    const sword::StartUnitFire& startMessage = fires_[ fireIdentifier ];
    const std::string targetTdentifier = resolver_.Resolve( startMessage.target().unit().id() );
    if( targetTdentifier.empty() )
        return;
    interactions::MunitionDetonation parameters;
    parameters.articulatedPartData = 0u;  // empty array
    parameters.detonationLocation = positions_[ targetTdentifier ];
    parameters.detonationResultCode = 1; // EntityImpact
    parameters.eventIdentifier.eventCount = static_cast< uint16 >( fireIdentifier );
    parameters.eventIdentifier.issuingObjectIdentifier = Omt13String( federateName_ );
    parameters.firingObjectIdentifier = Omt13String( boost::lexical_cast< std::string >( startMessage.firing_unit().id() ) );
    parameters.finalVelocityVector = rpr::VelocityVector( 0., 0., 700. );
    fires_.erase( fireIdentifier );
    interactionSender_.Send( parameters );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Created
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::Created( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Destroyed
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::Destroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Moved
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::Moved( const std::string& identifier, double latitude, double longitude )
{
    positions_[ identifier ] = rpr::WorldLocation( latitude, longitude, 0. );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::SideChanged
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::NameChanged
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::TypeChanged
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}
