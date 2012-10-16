// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AlatMonitoringEventListener.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KS_Perception.h"
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "alat monitoring disabled";
}

//-----------------------------------------------------------------------------
// Name: AlatMonitoringEventListener constructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
AlatMonitoringEventListener::AlatMonitoringEventListener( core::Facade& facade )
    : facade_( facade )
{
    facade.Register( event, *this );
}

//-----------------------------------------------------------------------------
// Name: AlatMonitoringEventListener destructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
AlatMonitoringEventListener::~AlatMonitoringEventListener()
{
    facade_.Unregister( event, *this );
}

// -----------------------------------------------------------------------------
// Name: AlatMonitoringEventListener::Notify
// Created: MCO 2012-04-26
// -----------------------------------------------------------------------------
void AlatMonitoringEventListener::Notify( const core::Model& event )
{
    event[ "entity/data" ].GetUserData< MIL_AgentPion >().GetKnowledge().GetKsPerception().MakePerceptionsAvailableTimed();
}
