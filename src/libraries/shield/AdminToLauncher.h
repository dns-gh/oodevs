// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_AdminToLauncher_h_
#define shield_AdminToLauncher_h_

#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include "proto/AdminToLauncher.pb.h"
#pragma warning( pop )

namespace shield
{
// =============================================================================
/** @class  AdminToLauncher
    @brief  Admin to launcher converter
*/
// Created: MCO 2010-12-01
// =============================================================================
class AdminToLauncher
{
public:
    //! @name Operations
    //@{
    static void Convert( const MsgsAdminToLauncher::MsgConnectionRequest& from, sword::ConnectionRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgSessionStartRequest& from, sword::SessionStartRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgSessionStopRequest& from, sword::SessionStopRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgExerciseListRequest& from, sword::ExerciseListRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgSessionListRequest& from, sword::SessionListRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgProfileListRequest& from, sword::ProfileListRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgConnectedProfileListRequest& from, sword::ConnectedProfileListRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgSessionParameterChangeRequest& from, sword::SessionParameterChangeRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgSessionCommandExecutionRequest& from, sword::SessionCommandExecutionRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgCheckpointListRequest& from, sword::CheckpointListRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgCheckpointDeleteRequest& from, sword::CheckpointDeleteRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgSessionNotification& from, sword::SessionNotificationRequest* to );
    //@}

private:
    static void ConvertParameter( const MsgsAdminToLauncher::SessionParameter& from, sword::SessionParameter* to);
    static void ConvertNotification( const MsgsAdminToLauncher::MsgSessionNotification::Notification& from, sword::SessionNotificationRequest::Notification* to );
    static void ConvertDirectoryChange( const MsgsAdminToLauncher::MsgSessionNotification::DirectoryChange& from, sword::SessionNotificationRequest::DirectoryChange* to );
};

}

#endif // shield_AdminToLauncher_h_
