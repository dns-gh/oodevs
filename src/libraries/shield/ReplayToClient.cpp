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

// -----------------------------------------------------------------------------
// Name: ReplayToClient::Convert
// Created: JSR 2011-07-22
// -----------------------------------------------------------------------------
void ReplayToClient::Convert( const sword::TimeTableRequestAck& from, MsgsReplayToClient::MsgTimeTableRequestAck* to )
{
    CONVERT_ENUM( error_code, ( sword::TimeTableRequestAck::no_error, MsgsReplayToClient::MsgTimeTableRequestAck::no_error )
                              ( sword::TimeTableRequestAck::invalid_tick_range, MsgsReplayToClient::MsgTimeTableRequestAck::invalid_tick_range ) );
}

// -----------------------------------------------------------------------------
// Name: ReplayToClient::Convert
// Created: JSR 2011-07-22
// -----------------------------------------------------------------------------
void ReplayToClient::Convert( const sword::TimeTable& from, MsgsReplayToClient::MsgTimeTable* to )
{
    for( int i = 0; i < from.time_table_item().size(); ++i )
    {
        MsgsReplayToClient::MsgTimeTable_TimeMapping* item = to->add_time_table_item();
        item->set_tick( from.time_table_item( i ).tick() );
        item->mutable_simulation_time()->set_data( from.time_table_item( i ).simulation_time().data() );
        item->mutable_real_time()->set_data( from.time_table_item( i ).real_time().data() );
    }
}

// -----------------------------------------------------------------------------
// Name: ReplayToClient::Convert
// Created: JSR 2011-07-22
// -----------------------------------------------------------------------------
void ReplayToClient::Convert( const sword::NewDataChunkNotification& from, MsgsReplayToClient::MsgNewDataChunkNotification* to )
{
    CONVERT( last_tick );
}
