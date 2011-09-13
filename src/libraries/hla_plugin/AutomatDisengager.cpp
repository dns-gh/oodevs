// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AutomatDisengager.h"
#include "ContextHandler_ABC.h"
#include "ContextFactory_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/SimulationPublisher_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AutomatDisengager constructor
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
AutomatDisengager::AutomatDisengager( ContextHandler_ABC< sword::AutomatCreation >& contextHandler, dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& contextFactory )
    : contextHandler_( contextHandler )
    , publisher_     ( publisher )
    , contextFactory_( contextFactory )
{
    contextHandler_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AutomatDisengager destructor
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
AutomatDisengager::~AutomatDisengager()
{
    contextHandler_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AutomatDisengager::Notify
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void AutomatDisengager::Notify( const sword::AutomatCreation& message, const std::string& /*identifier*/ )
{
    simulation::SetAutomatMode disengageMessage;
    disengageMessage().mutable_automate()->set_id( message.automat().id() );
    disengageMessage().set_mode( sword::disengaged );
    disengageMessage.Send( publisher_, contextFactory_.Create() );
}
