// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "IndirectFireReceiver.h"
#include "DotationTypeResolver_ABC.h"
#include "ContextFactory_ABC.h"
#include "Interactions.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "protocol/SimulationSenders.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: IndirectFireReceiver constructor
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
IndirectFireReceiver::IndirectFireReceiver( dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& factory, const DotationTypeResolver_ABC& resolver )
    : publisher_( publisher )
    , factory_  ( factory )
    , resolver_ ( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndirectFireReceiver destructor
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
IndirectFireReceiver::~IndirectFireReceiver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndirectFireReceiver::Receive
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void IndirectFireReceiver::Receive( interactions::MunitionDetonation& interaction )
{
    const std::string target = interaction.targetObjectIdentifier.str();
    if( !target.empty() )
        return;
    simulation::MagicAction message;
    message().set_type( sword::MagicAction::create_fire_order_on_location );
    sword::MissionParameter& locationParameter = *message().mutable_parameters()->add_elem();
    locationParameter.set_null_value( false );
    sword::Location& location = *locationParameter.add_value()->mutable_location();
    location.set_type( sword::Location::point );
    sword::CoordLatLong& coordinates = *location.mutable_coordinates()->add_elem();
    coordinates.set_latitude( interaction.detonationLocation.Latitude() );
    coordinates.set_longitude( interaction.detonationLocation.Longitude() );
    message().mutable_parameters()->add_elem()->add_value()->mutable_resourcetype()->set_id( resolver_.Resolve( interaction.munitionType ) );
    message().mutable_parameters()->add_elem()->add_value()->set_areal( 1. );  // iterations, hardcoded
    message.Send( publisher_, factory_.Create() );
}
