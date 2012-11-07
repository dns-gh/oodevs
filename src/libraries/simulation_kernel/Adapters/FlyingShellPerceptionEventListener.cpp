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
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "flying shell detection";
}

// -----------------------------------------------------------------------------
// Name: FlyingShellPerceptionEventListener constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
FlyingShellPerceptionEventListener::FlyingShellPerceptionEventListener( const core::Model& model, core::Facade& facade )
    : model_ ( model )
    , facade_( facade )
{
    facade.Register( event, *this );
}

// -----------------------------------------------------------------------------
// Name: FlyingShellPerceptionEventListener destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
FlyingShellPerceptionEventListener::~FlyingShellPerceptionEventListener()
{
    facade_.Unregister( event, *this );
}

// -----------------------------------------------------------------------------
// Name: FlyingShellPerceptionEventListener::Notify
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void FlyingShellPerceptionEventListener::Notify( const core::Model& perception )
{
    const MIL_AgentPion& entity = perception[ "entity" ].GetUserData< MIL_AgentPion >();
    const MIL_Effect_IndirectFire& flyingShell = perception[ "flying-shell" ].GetUserData< MIL_Effect_IndirectFire >();
    MIL_Report::PostEvent( entity, MIL_Report::eRC_ObservationTirIndirect, flyingShell );
}
