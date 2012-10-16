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
#include "Entities/Orders/MIL_Report.h"
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
MovementReportNameEventListener::MovementReportNameEventListener( core::Facade& facade )
    : facade_( facade )
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
    RoleAction_Moving& role = report[ "entity/data" ].GetUserData< MIL_AgentPion >().GetRole< RoleAction_Moving >();
    const int code = report[ "code" ];
    const std::string& name = report[ "name" ];
    MIL_Report::PostEvent( pion, static_cast< MIL_Report::E_EngineReport >( code ), name );
}
