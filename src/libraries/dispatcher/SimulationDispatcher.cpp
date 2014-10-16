// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "SimulationDispatcher.h"
#include "ReplayModel_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SimulationDispatcher::SimulationDispatcher( ClientPublisher_ABC& publisher, const ReplayModel_ABC& synchronizer )
    : publisher_( publisher )
    , synchronizer_( synchronizer )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SimulationDispatcher::~SimulationDispatcher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::IsDestruction
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool SimulationDispatcher::IsDestruction( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().has_unit_knowledge_destruction() ||
        wrapper.message().has_object_destruction() ||
        wrapper.message().has_unit_destruction() ||
        wrapper.message().has_object_knowledge_destruction() ||
        wrapper.message().has_log_medical_handling_destruction() ||
        wrapper.message().has_log_maintenance_handling_destruction() ||
        wrapper.message().has_log_supply_handling_destruction() ||
        wrapper.message().has_crowd_concentration_destruction() ||
        wrapper.message().has_crowd_flow_destruction() ||
        wrapper.message().has_crowd_knowledge_destruction() ||
        wrapper.message().has_crowd_concentration_knowledge_destruction() ||
        wrapper.message().has_crowd_flow_knowledge_destruction() ||
        wrapper.message().has_stop_fire_effect() ||
        wrapper.message().has_stop_unit_fire() ||
        wrapper.message().has_stop_crowd_fire() ||
        wrapper.message().has_invalidate_report();
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::Receive
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void SimulationDispatcher::Receive( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_control_begin_tick() )
    {
        const auto currentTick = wrapper.message().control_begin_tick().current_tick();
        MT_LOG_INFO_MSG( "New tick : " << ( currentTick ? currentTick : 2 ) );
    }
    if( !synchronizer_.IsSynching() || IsDestruction( wrapper )
        || ( wrapper.message().has_control_begin_tick() && wrapper.message().control_begin_tick().current_tick() == 0 )
        || ( wrapper.message().has_control_end_tick() && wrapper.message().control_end_tick().current_tick() == 0 ) )
        publisher_.Send( wrapper );
}
