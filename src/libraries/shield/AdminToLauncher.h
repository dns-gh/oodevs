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
    static void Convert( const MsgsAdminToLauncher::MsgControlStart& from, sword::ControlStartExercise* to );
    static void Convert( const MsgsAdminToLauncher::MsgControlStop& from, sword::ControlStopExercise* to );
    static void Convert( const MsgsAdminToLauncher::MsgExercicesListRequest& from, sword::ExercicesListRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgProfilesListRequest& from, sword::ProfilesListRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgConnectedProfilesListRequest& from, sword::MsgConnectedProfilesListRequest* to );
    static void Convert( const MsgsAdminToLauncher::MsgAdminDirectoryChangeNotification& from, sword::MsgAdminDirectoryChangeNotification* to );
    //@}
};

}

#endif // shield_AdminToLauncher_h_
