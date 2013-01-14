// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ReportEventListener.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Orders/MIL_Report.h"
#include <core/Model.h>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: ReportEventListener constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
ReportEventListener::ReportEventListener( core::Facade& facade )
    : EventListenerBase( facade, "report" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportEventListener destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
ReportEventListener::~ReportEventListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportEventListener::Notify
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void ReportEventListener::Notify( const core::Model& report )
{
    const MIL_AgentPion& pion = report[ "entity/data" ].GetUserData< MIL_AgentPion >();
    const std::string& code = report[ "code" ];
    MIL_Report::PostEvent( pion, code );
}
