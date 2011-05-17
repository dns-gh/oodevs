// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_LauncherToAdmin_h
#define shield_LauncherToAdmin_h

#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include "proto/LauncherToAdmin.pb.h"
#pragma warning( pop )

namespace shield
{
// =============================================================================
/** @class  LauncherToAdmin
    @brief  Launcher to admin converter
*/
// Created: MCO 2010-12-01
// =============================================================================
class LauncherToAdmin
{
public:
    //! @name Operations
    //@{
    static void Convert( const sword::ConnectionResponse& from, MsgsLauncherToAdmin::MsgConnectionResponse* to );
    static void Convert( const sword::ExerciseListResponse& from, MsgsLauncherToAdmin::MsgExerciseListResponse* to );
    static void Convert( const sword::SessionListResponse& from, MsgsLauncherToAdmin::MsgSessionListResponse* to );
    static void Convert( const sword::SessionStartResponse& from, MsgsLauncherToAdmin::MsgSessionStartResponse* to );
    static void Convert( const sword::SessionStopResponse& from, MsgsLauncherToAdmin::MsgSessionStopResponse* to );
    static void Convert( const sword::ProfileListResponse& from, MsgsLauncherToAdmin::MsgProfileListResponse* to );
    static void Convert( const sword::ConnectedProfileListResponse& from, MsgsLauncherToAdmin::MsgConnectedProfileListResponse* to );
    static void Convert( const sword::SessionStatus& from, MsgsLauncherToAdmin::MsgSessionStatus* to );
    static void Convert( const sword::SessionParameterChangeResponse& from, MsgsLauncherToAdmin::MsgSessionParameterChangeResponse* to );
    static void Convert( const sword::SessionCommandExecutionResponse& from, MsgsLauncherToAdmin::MsgSessionCommandExecutionResponse* to );
    static void Convert( const sword::CheckpointListResponse& from, MsgsLauncherToAdmin::MsgCheckpointListResponse* to );
    static void Convert( const sword::CheckpointDeleteResponse& from, MsgsLauncherToAdmin::MsgCheckpointDeleteResponse* to );
    static void Convert( const sword::SessionNotification& from, MsgsLauncherToAdmin::MsgSessionNotification* to );
    static void Convert( const sword::ControlInformation& from, MsgsLauncherToAdmin::MsgControlInformation* to );
    static void Convert( const sword::ClientConnectionNotification& from, MsgsLauncherToAdmin::MsgClientConnectionNotification* to );
    //@}
};

}

#endif // shield_LauncherToAdmin_h
