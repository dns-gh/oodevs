// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "SimulationTimeManager.h"
#include "protocol/SimulationSenders.h"

using namespace plugins::hla;

SimulationTimeManager::SimulationTimeManager( tools::MessageController_ABC< sword::SimToClient_Content >& messageController )
    : messageController_( messageController )
{
    CONNECT( messageController_, *this, control_begin_tick );
}

SimulationTimeManager::~SimulationTimeManager()
{
    DISCONNECT( messageController_, *this, control_begin_tick );
}

const std::string& SimulationTimeManager::getSimulationTime( ) const
{
    return simulationTime_;
}

unsigned int SimulationTimeManager::getSimulationtick() const
{
    return simulationTick_;
}

void SimulationTimeManager::Notify( const sword::ControlBeginTick& message, int )
{
    simulationTime_ = message.date_time().data();
    simulationTick_ = message.current_tick();
}
