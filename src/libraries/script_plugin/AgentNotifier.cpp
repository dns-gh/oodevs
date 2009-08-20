// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "AgentNotifier.h"
#include "AgentEvents.h"
#include "clients_kernel/Controller.h"

using namespace plugins::script;
using namespace events;

// -----------------------------------------------------------------------------
// Name: AgentNotifier constructor
// Created: AGE 2008-06-13
// -----------------------------------------------------------------------------
AgentNotifier::AgentNotifier( kernel::Controller& controller, const dispatcher::Agent& agent )
    : controller_( controller )
    , agent_     ( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentNotifier destructor
// Created: AGE 2008-06-13
// -----------------------------------------------------------------------------
AgentNotifier::~AgentNotifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentNotifier::DoUpdate
// Created: SBO 2009-08-19
// -----------------------------------------------------------------------------
void AgentNotifier::DoUpdate( const ASN1T_MsgUnitCreation& message )
{
    controller_.Update( AgentCreated( agent_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentNotifier::DoUpdate
// Created: AGE 2008-06-13
// -----------------------------------------------------------------------------
void AgentNotifier::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.positionPresent )
        controller_.Update( AgentMoved( agent_ ) );
    if( message.m.etat_operationnelPresent )
        controller_.Update( OperationalStateChanged( agent_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentNotifier::DoUpdate
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
void AgentNotifier::DoUpdate( const ASN1T_MsgUnitOrder& )
{
    controller_.Update( MissionStarted( agent_ ) );
}
