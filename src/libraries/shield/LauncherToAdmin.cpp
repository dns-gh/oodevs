// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "LauncherToAdmin.h"
#include "SimulationTools.h"

using namespace shield;

namespace
{
    template< typename From, typename To >
    void ConvertNetworkAddress( const From& from, To* to )
    {
        CONVERT( ip );
        CONVERT( port );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::ConnectionAck& from, MsgsLauncherToAdmin::MsgConnectionAck* to )
{
    CONVERT_ENUM( error_code, ( sword::ConnectionAck::success, MsgsLauncherToAdmin::MsgConnectionAck::success )
                              ( sword::ConnectionAck::invalid_connection, MsgsLauncherToAdmin::MsgConnectionAck::invalid_connection )
                              ( sword::ConnectionAck::incompatible_protocol_version, MsgsLauncherToAdmin::MsgConnectionAck::incompatible_protocol_version )
                              ( sword::ConnectionAck::exercise_already_running, MsgsLauncherToAdmin::MsgConnectionAck::exercise_already_running ) );
    CONVERT( context );
    if( from.has_server_version() && from.server_version().has_value() )
        to->mutable_server_version()->set_value( from.server_version().value() );
    CONVERT_CB_TO( dispatcher_address, ConvertNetworkAddress );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::ExercicesListResponse& from, MsgsLauncherToAdmin::MsgExercicesListResponse* to )
{
    CONVERT_ENUM( error_code, ( sword::ExercicesListResponse::success, MsgsLauncherToAdmin::MsgExercicesListResponse::success )
                              ( sword::ExercicesListResponse::failure, MsgsLauncherToAdmin::MsgExercicesListResponse::failure ) );
    for( int i = 0; i < from.exercise().size(); ++i )
        ConvertExercise( from.exercise( i ), to->add_exercise() );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::ControlStartExerciseAck& from, MsgsLauncherToAdmin::MsgControlStartAck* to )
{
    CONVERT_ENUM( error_code, ( sword::ControlStartExerciseAck::success, MsgsLauncherToAdmin::MsgControlStartAck::success )
                              ( sword::ControlStartExerciseAck::bad_exercise_name, MsgsLauncherToAdmin::MsgControlStartAck::bad_exercise_name )
                              ( sword::ControlStartExerciseAck::exercise_already_running, MsgsLauncherToAdmin::MsgControlStartAck::exercise_already_running )
                              ( sword::ControlStartExerciseAck::invalid_checkpoint, MsgsLauncherToAdmin::MsgControlStartAck::invalid_checkpoint ) );
    CONVERT_CB_TO( exercise, ConvertExercise );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::ControlStopExerciseAck& from, MsgsLauncherToAdmin::MsgControlStopAck* to )
{
    CONVERT_ENUM( error_code, ( sword::ControlStopExerciseAck::success, MsgsLauncherToAdmin::MsgControlStopAck::success )
                              ( sword::ControlStopExerciseAck::bad_exercise_name, MsgsLauncherToAdmin::MsgControlStopAck::bad_exercise_name )
                              ( sword::ControlStopExerciseAck::exercise_not_running, MsgsLauncherToAdmin::MsgControlStopAck::exercise_not_running ) );
    CONVERT_CB_TO( exercise, ConvertExercise );
    CONVERT_SIMULATION_STATE( simulation_state );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::ConnectedProfileList& from, MsgsLauncherToAdmin::MsgConnectedProfileList* to )
{
    CONVERT_ENUM( error_code, ( sword::ConnectedProfileList::success, MsgsLauncherToAdmin::MsgConnectedProfileList::success )
                              ( sword::ConnectedProfileList::failure, MsgsLauncherToAdmin::MsgConnectedProfileList::failure ) );
    CONVERT_ENUM( context, ( sword::ConnectedProfileList::spontaneous, MsgsLauncherToAdmin::MsgConnectedProfileList::spontaneous )
                           ( sword::ConnectedProfileList::upon_request, MsgsLauncherToAdmin::MsgConnectedProfileList::upon_request ) );
    for( int i = 0; i < from.exercise().size(); ++i )
        ConvertProfile( from.exercise( i ), to->add_exercise() );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::ProfileDescriptionList& from, MsgsAuthenticationToClient::MsgProfileDescriptionList* to )
{
    for( int i = 0; i < from.elem().size(); ++i )
        ConvertProfileDescription( from.elem( i ), to->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::SimulationComponentState& from, MsgsLauncherToAdmin::MsgSimulationComponentState* to )
{
    CONVERT_SIMULATION_STATE( simulation_state );
    CONVERT_ENUM( dispatcher_state, ( sword::available, Common::available )
                                    ( sword::unavailable, Common::unavailable ) );
}
