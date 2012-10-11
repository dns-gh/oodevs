// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MovementReportNameEventListener.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "RoleAction_Moving.h"
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "movement report with name";
}

// -----------------------------------------------------------------------------
// Name: MovementReportNameEventListener constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
MovementReportNameEventListener::MovementReportNameEventListener( const core::Model& model, core::Facade& facade )
    : model_ ( model )
    , facade_( facade )
{
    facade.Register( event, *this ); // $$$$ MCO 2012-06-15: rename class to MovementReportNameEventListener ?
}

// -----------------------------------------------------------------------------
// Name: MovementReportNameEventListener destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
MovementReportNameEventListener::~MovementReportNameEventListener()
{
    facade_.Unregister( event, *this );
}

// -----------------------------------------------------------------------------
// Name: MovementReportNameEventListener::Notify
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void MovementReportNameEventListener::Notify( const core::Model& report )
{
    const unsigned int entity = report[ "entity" ];
    RoleAction_Moving& role = model_[ "entities" ][ entity ][ "data" ].GetUserData< MIL_AgentPion >().GetRole< RoleAction_Moving >();
    role.SendRC( report[ "code" ], report[ "name" ] );
}
