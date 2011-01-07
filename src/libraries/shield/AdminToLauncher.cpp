// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "AdminToLauncher.h"
#include "CommonTools.h"

using namespace shield;

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgConnectionRequest& from, sword::ConnectionRequest* to )
{
    to->mutable_client_version()->set_value( from.client_version().value() );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgControlStart& from, sword::ControlStartExercise* to )
{
    ConvertExercise( from.exercise(), to->mutable_exercise() );
    CONVERT_ENUM( mode, ( MsgsAdminToLauncher::MsgControlStart::play, sword::ControlStartExercise::play )
                        ( MsgsAdminToLauncher::MsgControlStart::replay, sword::ControlStartExercise::replay ) );
    CONVERT( use_after_action_analysis );
    CONVERT( use_external_systems );
    CONVERT( checkpoint );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgControlStop& from, sword::ControlStopExercise* to )
{
    ConvertExercise( from.exercise(), to->mutable_exercise() );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgExercicesListRequest& from, sword::ExercicesListRequest* to )
{
    CONVERT( context );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgProfilesListRequest& from, sword::ProfilesListRequest* to )
{
    CONVERT( context );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgConnectedProfilesListRequest& from, sword::ConnectedProfilesListRequest* to )
{
    CONVERT( context );
}

// -----------------------------------------------------------------------------
// Name: AdminToLauncher::Convert
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void AdminToLauncher::Convert( const MsgsAdminToLauncher::MsgAdminDirectoryChangeNotification& /*from*/, sword::AdminDirectoryChangeNotification* /*to*/ )
{
    // NOTHING
}
