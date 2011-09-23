// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "IndirectFireSender.h"
#include "Omt13String.h"
#include "Interactions.h"
#include "InteractionSender_ABC.h"
#include "protocol/Simulation.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: IndirectFireSender constructor
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
IndirectFireSender::IndirectFireSender( InteractionSender_ABC< interactions::MunitionDetonation >& interactionSender,
                                        tools::MessageController_ABC< sword::SimToClient_Content >& controller, const std::string& federateName )
    : interactionSender_( interactionSender )
    , federateName_     ( federateName )
{
    CONNECT( controller, *this, start_unit_fire );
    CONNECT( controller, *this, stop_unit_fire );
}

// -----------------------------------------------------------------------------
// Name: IndirectFireSender destructor
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
IndirectFireSender::~IndirectFireSender()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndirectFireSender::Notify
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void IndirectFireSender::Notify( const sword::StartUnitFire& message, int /*context*/ )
{
    if( message.type() == sword::StartUnitFire::indirect )
        fires_[ message.fire().id() ] = message;
}

// -----------------------------------------------------------------------------
// Name: IndirectFireSender::Notify
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void IndirectFireSender::Notify( const sword::StopUnitFire& message, int /*context*/ )
{
    const unsigned int fireIdentifier = message.fire().id();
    if( fires_.find( fireIdentifier ) == fires_.end() )
        return;
    const sword::StartUnitFire& startMessage = fires_[ fireIdentifier ];
    interactions::MunitionDetonation parameters;
    parameters.articulatedPartData = 0u;  // empty array
    parameters.detonationLocation = rpr::WorldLocation( startMessage.target().position().latitude(), startMessage.target().position().longitude(), 0. );
    parameters.detonationResultCode = 3; // GroundImpact
    parameters.eventIdentifier.eventCount = static_cast< uint16 >( fireIdentifier );
    parameters.eventIdentifier.issuingObjectIdentifier = Omt13String( federateName_ );
    parameters.firingObjectIdentifier = Omt13String( boost::lexical_cast< std::string >( startMessage.firing_unit().id() ) );
    parameters.finalVelocityVector = rpr::VelocityVector( 0., 0., 400. );  // $$$$ _RC_ SLI 2011-09-23: Hardcoded
    parameters.fuseType = 0; // Other
    parameters.munitionObjectIdentifier = Omt13String();
    fires_.erase( fireIdentifier );
    interactionSender_.Send( parameters );
}
