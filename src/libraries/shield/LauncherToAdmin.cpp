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
void LauncherToAdmin::Convert( const sword::ConnectionResponse& from, MsgsLauncherToAdmin::MsgConnectionResponse* to )
{
    CONVERT_ENUM( error_code, ( sword::ConnectionResponse::success, MsgsLauncherToAdmin::MsgConnectionResponse::success )
                              ( sword::ConnectionResponse::incompatible_protocol_version, MsgsLauncherToAdmin::MsgConnectionResponse::incompatible_protocol_version ) );
    CONVERT_VERSION( server_version );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::ExerciseListResponse& from, MsgsLauncherToAdmin::MsgExerciseListResponse* to )
{
    CONVERT_SIMPLE_LIST( exercise, ConvertSimple );
//    for(int i=0; i< from.exercise().size(); ++i )
//        ConvertSimple(from.exercise(i), to->add_exercise() );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::SessionListResponse& from, MsgsLauncherToAdmin::MsgSessionListResponse* to )
{
    CONVERT_ENUM( error_code, ( sword::SessionListResponse::success, MsgsLauncherToAdmin::MsgSessionListResponse::success )
                              ( sword::SessionListResponse::invalid_exercise_name, MsgsLauncherToAdmin::MsgSessionListResponse::invalid_exercise_name ) );
    CONVERT( exercise );
    CONVERT_SIMPLE_LIST( session, ConvertSimple );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::SessionStartResponse& from, MsgsLauncherToAdmin::MsgSessionStartResponse* to )
{
    CONVERT_ENUM( type, ( sword::SessionStartResponse::simulation, MsgsLauncherToAdmin::MsgSessionStartResponse::simulation )
                        ( sword::SessionStartResponse::dispatch, MsgsLauncherToAdmin::MsgSessionStartResponse::dispatch )
                        ( sword::SessionStartResponse::replay, MsgsLauncherToAdmin::MsgSessionStartResponse::replay ) );
    CONVERT_ENUM( error_code, ( sword::SessionStartResponse::success, MsgsLauncherToAdmin::MsgSessionStartResponse::success )
                              ( sword::SessionStartResponse::invalid_exercise_name, MsgsLauncherToAdmin::MsgSessionStartResponse::invalid_exercise_name )
                              ( sword::SessionStartResponse::invalid_checkpoint, MsgsLauncherToAdmin::MsgSessionStartResponse::invalid_checkpoint )
                              ( sword::SessionStartResponse::session_already_running, MsgsLauncherToAdmin::MsgSessionStartResponse::session_already_running ) );
    CONVERT( exercise );
    CONVERT( session );
    CONVERT( checkpoint );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::SessionStopResponse& from, MsgsLauncherToAdmin::MsgSessionStopResponse* to )
{
    CONVERT_ENUM( error_code, ( sword::SessionStopResponse::success, MsgsLauncherToAdmin::MsgSessionStopResponse::success )
                              ( sword::SessionStopResponse::invalid_exercise_name, MsgsLauncherToAdmin::MsgSessionStopResponse::invalid_exercise_name )
                              ( sword::SessionStopResponse::invalid_session_name, MsgsLauncherToAdmin::MsgSessionStopResponse::invalid_session_name )
                              ( sword::SessionStopResponse::session_not_running, MsgsLauncherToAdmin::MsgSessionStopResponse::session_not_running ) );
    CONVERT( exercise );
    CONVERT( session );
}

namespace
{
    template< typename From, typename To >
    void LocalConvertProfile( const From& from, To* to )
    {
        ConvertProfileDescription( from, to );
        CONVERT_LIST( read_only_formations, elem, ConvertIdentifier );
        CONVERT_LIST( read_write_formations, elem, ConvertIdentifier );
        CONVERT_LIST_TO( read_only_automates, read_only_automata, elem, ConvertIdentifier );
        CONVERT_LIST_TO( read_write_automates, read_write_automata, elem, ConvertIdentifier );
        CONVERT_LIST( read_only_parties, elem, ConvertIdentifier );
        CONVERT_LIST( read_write_parties, elem, ConvertIdentifier );
        CONVERT_LIST( read_only_crowds, elem, ConvertIdentifier );
        CONVERT_LIST( read_write_crowds, elem, ConvertIdentifier );
        // TODO AHC write_populations
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::ProfileListResponse& from, MsgsLauncherToAdmin::MsgProfileListResponse* to )
{
    CONVERT_ENUM( error_code, ( sword::ProfileListResponse::success, MsgsLauncherToAdmin::MsgProfileListResponse::success )
                              ( sword::ProfileListResponse::invalid_exercise_name, MsgsLauncherToAdmin::MsgProfileListResponse::invalid_exercise_name ) );
    CONVERT( exercise );
    CONVERT_SIMPLE_LIST(profile, LocalConvertProfile);
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::ConnectedProfileListResponse& from, MsgsLauncherToAdmin::MsgConnectedProfileListResponse* to )
{
    CONVERT_ENUM( error_code, ( sword::ConnectedProfileListResponse::success, MsgsLauncherToAdmin::MsgConnectedProfileListResponse::success )
                              ( sword::ConnectedProfileListResponse::invalid_exercise_name, MsgsLauncherToAdmin::MsgConnectedProfileListResponse::invalid_exercise_name )
                              ( sword::ConnectedProfileListResponse::invalid_session_name, MsgsLauncherToAdmin::MsgConnectedProfileListResponse::invalid_session_name )
                              ( sword::ConnectedProfileListResponse::session_not_running, MsgsLauncherToAdmin::MsgConnectedProfileListResponse::session_not_running ) );
    CONVERT( exercise );
    CONVERT( session );
    CONVERT_SIMPLE_LIST(profile, LocalConvertProfile);
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::SessionStatus& from, MsgsLauncherToAdmin::MsgSessionStatus* to )
{
    CONVERT_ENUM( status, ( sword::SessionStatus::starting, MsgsLauncherToAdmin::MsgSessionStatus::starting )
                              ( sword::SessionStatus::running, MsgsLauncherToAdmin::MsgSessionStatus::running )
                              ( sword::SessionStatus::paused, MsgsLauncherToAdmin::MsgSessionStatus::paused )
                              ( sword::SessionStatus::not_running, MsgsLauncherToAdmin::MsgSessionStatus::not_running ) );
    CONVERT( exercise );
    CONVERT( session );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::SessionParameterChangeResponse& from, MsgsLauncherToAdmin::MsgSessionParameterChangeResponse* to )
{
    CONVERT_ENUM( error_code, ( sword::SessionParameterChangeResponse::success, MsgsLauncherToAdmin::MsgSessionParameterChangeResponse::success )
                              ( sword::SessionParameterChangeResponse::invalid_exercise_name, MsgsLauncherToAdmin::MsgSessionParameterChangeResponse::invalid_exercise_name )
                              ( sword::SessionParameterChangeResponse::invalid_session_name, MsgsLauncherToAdmin::MsgSessionParameterChangeResponse::invalid_session_name )
                              ( sword::SessionParameterChangeResponse::session_not_running, MsgsLauncherToAdmin::MsgSessionParameterChangeResponse::session_not_running ) );
    CONVERT( exercise );
    CONVERT( session );
    CONVERT( checkpoint_frequency );
    CONVERT( acceleration_factor );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::SessionCommandExecutionResponse& from, MsgsLauncherToAdmin::MsgSessionCommandExecutionResponse* to )
{
    CONVERT_ENUM( error_code, ( sword::SessionCommandExecutionResponse::success, MsgsLauncherToAdmin::MsgSessionCommandExecutionResponse::success )
                              ( sword::SessionCommandExecutionResponse::invalid_exercise_name, MsgsLauncherToAdmin::MsgSessionCommandExecutionResponse::invalid_exercise_name )
                              ( sword::SessionCommandExecutionResponse::invalid_session_name, MsgsLauncherToAdmin::MsgSessionCommandExecutionResponse::invalid_session_name )
                              ( sword::SessionCommandExecutionResponse::session_not_running, MsgsLauncherToAdmin::MsgSessionCommandExecutionResponse::session_not_running )
                              ( sword::SessionCommandExecutionResponse::invalid_checkpoint, MsgsLauncherToAdmin::MsgSessionCommandExecutionResponse::invalid_checkpoint )
                              ( sword::SessionCommandExecutionResponse::session_already_paused, MsgsLauncherToAdmin::MsgSessionCommandExecutionResponse::session_already_paused )
                              ( sword::SessionCommandExecutionResponse::session_already_running, MsgsLauncherToAdmin::MsgSessionCommandExecutionResponse::session_already_running )
                              ( sword::SessionCommandExecutionResponse::invalid_date_time, MsgsLauncherToAdmin::MsgSessionCommandExecutionResponse::invalid_date_time ) );
    CONVERT( exercise );
    CONVERT( session );
    CONVERT( saved_checkpoint );
    CONVERT( running );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::CheckpointListResponse& from, MsgsLauncherToAdmin::MsgCheckpointListResponse* to )
{
    CONVERT_ENUM( error_code, ( sword::CheckpointListResponse::success, MsgsLauncherToAdmin::MsgCheckpointListResponse::success )
                              ( sword::CheckpointListResponse::invalid_exercise_name, MsgsLauncherToAdmin::MsgCheckpointListResponse::invalid_exercise_name )
                              ( sword::CheckpointListResponse::invalid_session_name, MsgsLauncherToAdmin::MsgCheckpointListResponse::invalid_session_name ) );
    CONVERT( exercise );
    CONVERT( session );
    CONVERT_SIMPLE_LIST( checkpoint, ConvertSimple );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::CheckpointDeleteResponse& from, MsgsLauncherToAdmin::MsgCheckpointDeleteResponse* to )
{
    CONVERT_ENUM( error_code, ( sword::CheckpointDeleteResponse::success, MsgsLauncherToAdmin::MsgCheckpointDeleteResponse::success )
                              ( sword::CheckpointDeleteResponse::invalid_exercise_name, MsgsLauncherToAdmin::MsgCheckpointDeleteResponse::invalid_exercise_name )
                              ( sword::CheckpointDeleteResponse::invalid_session_name, MsgsLauncherToAdmin::MsgCheckpointDeleteResponse::invalid_session_name ) );
    CONVERT( exercise );
    CONVERT( session );
    CONVERT_SIMPLE_LIST( checkpoint, ConvertSimple );
}

namespace
{
    void ConvertUnitUpdate( const sword::SessionNotification::UnitUpdate& from, MsgsLauncherToAdmin::MsgSessionNotification::UnitUpdate* to )
    {
        if( from.has_extensions() )
            for( int j = 0; j < from.extensions().entries().size(); ++j )
                ConvertExtensionEntry( from.extensions().entries( j ), to->mutable_extensions()->add_entries() );
    }
    void ConvertFormationUpdate( const sword::SessionNotification::FormationUpdate& from, MsgsLauncherToAdmin::MsgSessionNotification::FormationUpdate* to )
    {
        if( from.has_extensions() )
            for( int j = 0; j < from.extensions().entries().size(); ++j )
                ConvertExtensionEntry( from.extensions().entries( j ), to->mutable_extensions()->add_entries() );
    }
    void ConvertProfileCreation( const sword::SessionNotification::ProfileCreation& from, MsgsLauncherToAdmin::MsgSessionNotification::ProfileCreation* to )
    {
        CONVERT_CB( profile, ConvertProfileDescription );
    }
    void ConvertProfileUpdate( const sword::SessionNotification::ProfileUpdate& from, MsgsLauncherToAdmin::MsgSessionNotification::ProfileUpdate* to )
    {
        CONVERT_CB( profile, ConvertProfileDescription );
    }

    void ConvertNotification( const sword::SessionNotification::Notification& from, MsgsLauncherToAdmin::MsgSessionNotification::Notification* to )
    {
        CONVERT_CB( unit_update, ConvertUnitUpdate );
        CONVERT_CB( formation_update, ConvertFormationUpdate );
        CONVERT_CB( profile_creation, ConvertProfileCreation );
        CONVERT_CB( profile_update, ConvertProfileUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::SessionNotification& from, MsgsLauncherToAdmin::MsgSessionNotification* to )
{
    CONVERT( exercise );
    CONVERT( session );
    CONVERT_CB( notification, ConvertNotification );
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::ControlInformation& from, MsgsLauncherToAdmin::MsgControlInformation* to )
{
    CONVERT( current_tick );
    CONVERT( tick_duration );
    CONVERT( time_factor );
    to->set_pathfind_request_number( 0 ); // TODO AHC
    to->set_last_checkpoint_build_duration( 0 ); // TODO AHC
}

namespace
{
    void ConvertConnectionNotification(const sword::ClientConnectionNotification::ClientConnection& from, MsgsLauncherToAdmin::MsgClientConnectionNotification::ClientConnection* to)
    {
        CONVERT( login );
        CONVERT( connected );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherToAdmin::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void LauncherToAdmin::Convert( const sword::ClientConnectionNotification& from, MsgsLauncherToAdmin::MsgClientConnectionNotification* to )
{
    CONVERT( exercise );
    CONVERT( session );
    CONVERT_SIMPLE_LIST( connection, ConvertConnectionNotification );
}
