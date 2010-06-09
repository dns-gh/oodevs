// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "AutomatNotifier.h"
#include "AgentEvents.h"
#include "clients_kernel/Controller.h"
#include "protocol/protocol.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: AutomatNotifier constructor
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
AutomatNotifier::AutomatNotifier( kernel::Controller& controller, const dispatcher::Automat& automat )
    : controller_( controller )
    , automat_   ( automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatNotifier destructor
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
AutomatNotifier::~AutomatNotifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatNotifier::DoUpdate
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
void AutomatNotifier::DoUpdate( const Common::MsgAutomatOrder& )
{
    controller_.Update( events::AutomatMissionStarted( automat_ ) );
}
