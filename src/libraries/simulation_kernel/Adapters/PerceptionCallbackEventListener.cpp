// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PerceptionCallbackEventListener.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Decision_ABC.h"
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "perception callback";
}

// -----------------------------------------------------------------------------
// Name: PerceptionCallbackEventListener constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
PerceptionCallbackEventListener::PerceptionCallbackEventListener( core::Facade& facade )
    : facade_( facade )
{
    facade.Register( event, *this );
}

// -----------------------------------------------------------------------------
// Name: PerceptionCallbackEventListener destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
PerceptionCallbackEventListener::~PerceptionCallbackEventListener()
{
    facade_.Unregister( event, *this );
}

// -----------------------------------------------------------------------------
// Name: PerceptionCallbackEventListener::Notify
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void PerceptionCallbackEventListener::Notify( const core::Model& callback )
{
    DEC_Decision_ABC& role = callback[ "entity/data" ].GetUserData< MIL_AgentPion >().GetRole< DEC_Decision_ABC >();
    role.CallbackPerception( callback[ "perception" ] );
}
