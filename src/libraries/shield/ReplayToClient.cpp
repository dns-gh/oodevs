// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ReplayToClient.h"
#include "SimulationTools.h"
#include "Tools.h"

using namespace shield;

// -----------------------------------------------------------------------------
// Name: ReplayToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void ReplayToClient::Convert( const sword::ControlReplayInformation& from, MsgsReplayToClient::MsgControlReplayInformation* to )
{
    CONVERT( current_tick );
    CONVERT_DATE( initial_date_time );
    CONVERT_DATE( date_time );
    CONVERT( tick_duration );
    CONVERT( time_factor );
    CONVERT_SIMULATION_STATE( status );
    CONVERT( tick_count );
}

// -----------------------------------------------------------------------------
// Name: ReplayToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void ReplayToClient::Convert( const sword::ControlSkipToTickAck& from, MsgsReplayToClient::MsgControlSkipToTickAck* to )
{
    CONVERT( tick );
    ConvertControlAckErrorCode( from, to );
}
