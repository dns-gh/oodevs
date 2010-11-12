// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Launcher_dll_pch.h"
#include "ProcessService.h"
#include "frontend/commands.h"
#include "frontend/Config.h"
#include "frontend/ConfigurationManipulator.h"
#include "frontend/CreateSession.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/StartExercise.h"
#include "frontend/StartReplay.h"
#include "protocol/LauncherSenders.h"
#include <qstringlist.h>

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: ProcessService constructor
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
ProcessService::ProcessService( const tools::GeneralConfig& config )
    : config_( config )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProcessService destructor
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
ProcessService::~ProcessService()
{
    while( ! processes_.empty() )
    {
        boost::shared_ptr< frontend::ProcessWrapper > process;
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        for( std::map< std::string, boost::weak_ptr< frontend::ProcessWrapper > >::iterator it = processes_.begin(); it != processes_.end(); ++it )
            if( ! it->second.expired() )
            {
                boost::shared_ptr< frontend::ProcessWrapper > weak( it->second );
                process = weak;
                break;
            }
        if( process.get() )
            process->Stop();
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendExerciseList
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void ProcessService::SendExerciseList( MsgsLauncherToAdmin::MsgExercicesListResponse& message )
{
    const QStringList exercises = frontend::commands::ListExercises( config_ );
    for( QStringList::const_iterator it = exercises.begin(); it != exercises.end(); ++it )
    {
        const std::string name = (*it).ascii();
        Common::MsgExercise& exercise = *message.mutable_exercise()->Add();
        exercise.set_name( name );
        exercise.set_running( IsRunning( name ) );
        if( exercise.running() )
            exercise.set_port( 10001 ); // $$$$ SBO 2010-10-26: TODO: handle port configuration/multiple exercises
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::StartExercise
// Created: SBO 2010-10-07
// -----------------------------------------------------------------------------
MsgsLauncherToAdmin::MsgControlStartAck::ErrorCode ProcessService::StartExercise( const MsgsAdminToLauncher::MsgControlStart& message )
{
    const std::string session = "multiplayer"; // $$$$ SBO 2010-10-28: TODO: add session to message
    const std::string exercise = message.exercise().name();
    const std::string checkpoint = message.has_checkpoint() ? message.checkpoint() : "";
    if( ! frontend::commands::ExerciseExists( config_, exercise ) )
        return MsgsLauncherToAdmin::MsgControlStartAck::bad_exercise_name;
    if( IsRunning( exercise ) )
        return MsgsLauncherToAdmin::MsgControlStartAck::exercise_already_running;
    if( message.has_checkpoint() && ! frontend::commands::CheckpointExists( config_, exercise, session, checkpoint ) )
        return MsgsLauncherToAdmin::MsgControlStartAck::invalid_checkpoint;
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetDefaultValues();
        if( message.has_use_after_action_analysis() && message.use_after_action_analysis() )
            action.SetOption( "session/config/dispatcher/plugins/recorder", "" );
        action.Commit();
    }
    boost::shared_ptr< frontend::SpawnCommand > command;
    if( message.mode() == MsgsAdminToLauncher::MsgControlStart::play )
        command.reset( new frontend::StartExercise( config_, exercise.c_str(), session.c_str(), checkpoint.c_str(), true ) );
    else
        command.reset( new frontend::StartReplay( config_, exercise.c_str(), session.c_str(), 10001, true ) );
    boost::shared_ptr< frontend::ProcessWrapper > wrapper( new frontend::ProcessWrapper( *this, command ) );
    {
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        processes_[ exercise ] = wrapper;
    }
    wrapper->Start();
    return MsgsLauncherToAdmin::MsgControlStartAck::success;
}

// -----------------------------------------------------------------------------
// Name: ProcessService::StopExercise
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
MsgsLauncherToAdmin::MsgControlStopAck::ErrorCode ProcessService::StopExercise( const MsgsAdminToLauncher::MsgControlStop& message )
{
    const std::string name = message.exercise().name();
    std::map< std::string, boost::weak_ptr< frontend::ProcessWrapper > >::iterator it = processes_.find( name );
    if( it != processes_.end() && ! it->second.expired() )
    {
        boost::shared_ptr< frontend::ProcessWrapper > process( it->second );
        process->Stop();
        return MsgsLauncherToAdmin::MsgControlStopAck::success;
    }
    return frontend::commands::ExerciseExists( config_, name ) ? MsgsLauncherToAdmin::MsgControlStopAck::exercise_not_running 
                                                               : MsgsLauncherToAdmin::MsgControlStopAck::bad_exercise_name;
}

// -----------------------------------------------------------------------------
// Name: ProcessService::IsRunning
// Created: SBO 2010-10-07
// -----------------------------------------------------------------------------
bool ProcessService::IsRunning( const std::string& exercise ) const
{
    std::map< std::string, boost::weak_ptr< frontend::ProcessWrapper > >::const_iterator it = processes_.find( exercise );
    return it != processes_.end() && ! it->second.expired();
}

// -----------------------------------------------------------------------------
// Name: ProcessService::ProcessStopped
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void ProcessService::ProcessStopped()
{
    boost::recursive_mutex::scoped_lock locker( mutex_ );
    for( std::map< std::string, boost::weak_ptr< frontend::ProcessWrapper > >::iterator it = processes_.begin(); it != processes_.end(); )
        if( it->second.expired() )
            it = processes_.erase( it );
        else
            ++it;
}
