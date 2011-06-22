// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "AdminToLauncher.h"
#include "ClientTools.h"

using namespace shield;

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgConnectionRequest& from, sword::ConnectionRequest* to )
{
    CONVERT_VERSION( client_version );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgSessionStartRequest& from, sword::SessionStartRequest* to )
{
    CONVERT( exercise );
    CONVERT_ENUM( type, ( MsgsAdminToLauncher::MsgSessionStartRequest::simulation, sword::SessionStartRequest::simulation )
                        ( MsgsAdminToLauncher::MsgSessionStartRequest::dispatch, sword::SessionStartRequest::dispatch )
                        ( MsgsAdminToLauncher::MsgSessionStartRequest::replay, sword::SessionStartRequest::replay ) );
    CONVERT( session );
    CONVERT( checkpoint );
    CONVERT_SIMPLE_LIST( parameter, ConvertParameter );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::ConvertParameter
// Created: AHC 2011-05-13
// -----------------------------------------------------------------------------
void AdminToLauncher::ConvertParameter( const MsgsAdminToLauncher::SessionParameter& from, sword::SessionParameter* to)
{
    CONVERT( key );
    CONVERT( value );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgSessionStopRequest& from, sword::SessionStopRequest* to )
{
    CONVERT( exercise );
    CONVERT( session );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: AC 2011-05-12
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgSessionListRequest& from, sword::SessionListRequest* to )
{
    CONVERT( exercise );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgExerciseListRequest& /*from*/, sword::ExerciseListRequest* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgProfileListRequest& from, sword::ProfileListRequest* to )
{
    CONVERT( exercise );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgConnectedProfileListRequest& from, sword::ConnectedProfileListRequest* to )
{
    CONVERT( exercise );
    CONVERT( session );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgSessionParameterChangeRequest& from, sword::SessionParameterChangeRequest* to )
{
    CONVERT( exercise );
    CONVERT( session );
    CONVERT( checkpoint_frequency );
    CONVERT( acceleration_factor );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgSessionCommandExecutionRequest& from, sword::SessionCommandExecutionRequest* to )
{
    CONVERT( exercise );
    CONVERT( session );
    CONVERT( save_checkpoint );
    CONVERT( set_running );
    CONVERT_DATE( time_change );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgCheckpointListRequest& from, sword::CheckpointListRequest* to )
{
    CONVERT( exercise );
    CONVERT( session );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgCheckpointDeleteRequest& from, sword::CheckpointDeleteRequest* to )
{
    CONVERT( exercise );
    CONVERT( session );
    CONVERT( checkpoint );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgSessionNotification& from, sword::SessionNotificationRequest* to )
{
    CONVERT( exercise );
    CONVERT( session );
    CONVERT_CB( notification, ConvertNotification );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::ConvertNotification
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void AdminToLauncher::ConvertNotification( const MsgsAdminToLauncher::MsgSessionNotification::Notification& from, sword::SessionNotificationRequest::Notification* to )
{
    CONVERT_CB( directory_change, ConvertDirectoryChange );
}
// -----------------------------------------------------------------------------
// Name: AdminToLauncher::ConvertNotification
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void AdminToLauncher::ConvertDirectoryChange( const MsgsAdminToLauncher::MsgSessionNotification::DirectoryChange& /*from*/, sword::SessionNotificationRequest::DirectoryChange* /*to*/ )
{
    // NOTHING
}
