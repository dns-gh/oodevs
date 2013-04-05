// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MovementReportEventListener.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Orders/MIL_Report.h"
#include <core/Model.h>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: MovementReportEventListener constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
MovementReportEventListener::MovementReportEventListener( core::Facade& facade )
    : EventListenerBase( facade, "movement report" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MovementReportEventListener destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
MovementReportEventListener::~MovementReportEventListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MovementReportEventListener::Notify
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void MovementReportEventListener::Notify( const core::Model& report )
{
    MIL_AgentPion& pion = report[ "entity/data" ].GetUserData< MIL_AgentPion >();
    const std::string& code = report[ "code" ];
    MIL_Report::PostEvent( pion, code );
}