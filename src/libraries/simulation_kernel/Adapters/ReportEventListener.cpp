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
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "report";
}

// -----------------------------------------------------------------------------
// Name: ReportEventListener constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
ReportEventListener::ReportEventListener( const core::Model& model, core::Facade& facade )
    : model_ ( model )
    , facade_( facade )
{
    facade.Register( event, *this );
}

// -----------------------------------------------------------------------------
// Name: ReportEventListener destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
ReportEventListener::~ReportEventListener()
{
    facade_.Unregister( event, *this );
}

// -----------------------------------------------------------------------------
// Name: ReportEventListener::Notify
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void ReportEventListener::Notify( const core::Model& report )
{
    const size_t identifier = report[ "entity" ];
    const core::Model& entity = model_[ "entities" ][ identifier ];
    const MIL_AgentPion& pion = entity[ "data" ].GetUserData< MIL_AgentPion >();
    const int code = report[ "code" ];
    MIL_Report::PostEvent( pion, static_cast< MIL_Report::E_EngineReport >( code ) );
}
