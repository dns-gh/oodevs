// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "DirectFireReceiver.h"
#include "RemoteAgentResolver_ABC.h"
#include "Interactions.h"
#include "ContextFactory_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/SimulationPublisher_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DirectFireReceiver constructor
// Created: SLI 2011-09-26
// -----------------------------------------------------------------------------
DirectFireReceiver::DirectFireReceiver( dispatcher::SimulationPublisher_ABC& publisher, const RemoteAgentResolver_ABC& resolver, const ContextFactory_ABC& factory )
    : publisher_( publisher )
    , resolver_ ( resolver )
    , factory_  ( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireReceiver destructor
// Created: SLI 2011-09-26
// -----------------------------------------------------------------------------
DirectFireReceiver::~DirectFireReceiver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireReceiver::Receive
// Created: SLI 2011-09-26
// -----------------------------------------------------------------------------
void DirectFireReceiver::Receive( interactions::MunitionDetonation& interaction )
{
    if( interaction.detonationResultCode != 1 ) // entity impact
        return;
    const std::string source = interaction.firingObjectIdentifier.str();
    const std::string target = interaction.targetObjectIdentifier.str();
    if( source.empty() || target.empty() )
        return;
    const unsigned int sourceIdentifier = resolver_.Resolve( source );
    if( sourceIdentifier == 0 )
        return;
    const unsigned int targetIdentifier = resolver_.Resolve( target );
    if( targetIdentifier == 0 )
        return;
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_unit()->set_id( sourceIdentifier );
    message().set_type( sword::UnitMagicAction::create_direct_fire_order );
    sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
    parameter.set_null_value( false );
    parameter.add_value()->mutable_agent()->set_id( targetIdentifier );
    message.Send( publisher_, factory_.Create() );
}
