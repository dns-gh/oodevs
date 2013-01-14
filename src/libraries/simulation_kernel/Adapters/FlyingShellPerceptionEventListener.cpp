// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FlyingShellPerceptionEventListener.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include <core/Model.h>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: FlyingShellPerceptionEventListener constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
FlyingShellPerceptionEventListener::FlyingShellPerceptionEventListener( core::Facade& facade )
    : EventListenerBase( facade, "flying shell detection" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FlyingShellPerceptionEventListener destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
FlyingShellPerceptionEventListener::~FlyingShellPerceptionEventListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FlyingShellPerceptionEventListener::Notify
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void FlyingShellPerceptionEventListener::Notify( const core::Model& perception )
{
    const MIL_AgentPion& entity = perception[ "entity/data" ].GetUserData< MIL_AgentPion >();
    const MIL_Effect_IndirectFire& flyingShell = perception[ "flying-shell/data" ].GetUserData< MIL_Effect_IndirectFire >();
    MIL_Report::PostEvent( entity, report::eRC_ObservationTirIndirect, flyingShell );
}
