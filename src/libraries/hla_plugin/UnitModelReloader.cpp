// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "UnitModelReloader.h"
#include "ContextHandler_ABC.h"
#include "ContextFactory_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/SimulationPublisher_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: UnitModelReloader constructor
// Created: SLI 2011-09-20
// -----------------------------------------------------------------------------
UnitModelReloader::UnitModelReloader( ContextHandler_ABC< sword::UnitCreation >& contextHandler, dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& contextFactory )
    : contextHandler_( contextHandler )
    , publisher_     ( publisher )
    , contextFactory_( contextFactory )
{
    contextHandler_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitModelReloader destructor
// Created: SLI 2011-09-20
// -----------------------------------------------------------------------------
UnitModelReloader::~UnitModelReloader()
{
    contextHandler_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitModelReloader::Notify
// Created: SLI 2011-09-20
// -----------------------------------------------------------------------------
void UnitModelReloader::Notify( const sword::UnitCreation& message, const std::string& /*identifier*/ )
{
    simulation::UnitMagicAction reloadMessage;
    reloadMessage().mutable_tasker()->mutable_unit()->set_id( message.unit().id() );
    reloadMessage().set_type( sword::reload_brain );
    sword::MissionParameter& parameter = *reloadMessage().mutable_parameters()->add_elem();
    parameter.set_null_value( false );
    parameter.add_value()->set_acharstr( "Strategic Bomber Patrol" );
    reloadMessage.Send( publisher_, contextFactory_.Create() );
}
