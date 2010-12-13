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
#include "proto/LauncherToAdmin.pb.h"

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
    static void Convert( const sword::ConnectionAck& from, MsgsLauncherToAdmin::MsgConnectionAck* to );
    static void Convert( const sword::ExercicesListResponse& from, MsgsLauncherToAdmin::MsgExercicesListResponse* to );
    static void Convert( const sword::ControlStartExerciseAck& from, MsgsLauncherToAdmin::MsgControlStartAck* to );
    static void Convert( const sword::ControlStopExerciseAck& from, MsgsLauncherToAdmin::MsgControlStopAck* to );
    static void Convert( const sword::MsgConnectedProfileList& from, MsgsLauncherToAdmin::MsgConnectedProfileList* to );
    static void Convert( const sword::ProfileDescriptionList& from, MsgsAuthenticationToClient::MsgProfileDescriptionList* to );
    static void Convert( const sword::MsgSimulationComponentState& from, MsgsLauncherToAdmin::MsgSimulationComponentState* to );
    //@}
};

}

#endif // shield_LauncherToAdmin_h
