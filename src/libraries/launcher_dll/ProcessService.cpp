// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "ProcessService.h"
#include "frontend/commands.h"
#include "frontend/Config.h"
#include "frontend/CreateSession.h"
#include "frontend/Profile.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/StartExercise.h"
#include "frontend/StartReplay.h"
#include "client_proxy/SwordMessageHandler_ABC.h"
#include "SwordFacade.h"
#include "Config.h"
#include "LauncherPublisher.h"
#include "LauncherService.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: ProcessService constructor
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
ProcessService::ProcessService( const launcher::Config& config, const tools::Loader_ABC& fileLoader, const LauncherService& server )
    : config_    ( config )
    , fileLoader_( fileLoader )
    , server_    ( server )
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
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); )
        {
            if( it->second->IsRunning() )
            {
                it->second->Stop();
                break;
            }
            it = processes_.erase( it );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendExerciseList
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void ProcessService::SendExerciseList( sword::ExerciseListResponse& message )
{
    const tools::Path::T_Paths exercises = frontend::commands::ListExercises( config_ );
    for( auto it = exercises.begin(); it != exercises.end(); ++it )
        message.add_exercise( it->ToUTF8() );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendRunningExercices
// Created: SLI 2011-07-22
// -----------------------------------------------------------------------------
void ProcessService::SendRunningExercices( const std::string& endpoint ) const
{
    for( ProcessContainer::const_iterator it = processes_.begin(); it != processes_.end(); ++it )
    {
        const std::pair< tools::Path, tools::Path >& key = it->first;
        if( IsRunning( key.first, key.second ) )
        {
            SessionStatus message;
            message().set_exercise( key.first.ToUTF8() );
            message().set_session( key.second.ToUTF8() );
            message().set_status( sword::SessionStatus::running );
            message.Send( server_.ResolveClient( endpoint ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::IsRunning
// Created: SBO 2010-10-07
// -----------------------------------------------------------------------------
bool ProcessService::IsRunning( const tools::Path& exercise, const tools::Path& session ) const
{
    auto it = processes_.find( std::make_pair( exercise, session ) );
    return it != processes_.end() && ( config_.GetTestMode() || it->second->IsRunning() );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::NotifyStopped
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void ProcessService::NotifyStopped()
{
    boost::recursive_mutex::scoped_lock locker( mutex_ );
    for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); )
        if( !it->second->IsRunning() )
            it = processes_.erase( it );
        else
            ++it;
}

namespace
{
    void NotifyError( const std::string& endpoint,
                      const LauncherService& server,
                      ProcessService::ProcessContainer& processes )
    {
        if( endpoint.empty() )
            return;
        for( auto cur = processes.begin(); cur != processes.end(); ++cur )
        {
            const auto spawns = cur->second->GetProcess()->GetSpawns();
            for( auto it = spawns.begin(); it != spawns.end(); ++it )
            {
                auto cmd = *it;
                if( cmd->GetName() != endpoint )
                    continue;
                SessionStatus msg;
                msg().set_status( sword::SessionStatus::not_running );
                msg().set_exercise( cmd->GetExercise().ToUTF8() );
                msg().set_session( cmd->GetSession().ToUTF8() );
                msg.Send( server.ResolveClient( endpoint ) );
                processes.erase( cur );
                return;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::NotifyError
// Created: SBO 2010-12-09
// -----------------------------------------------------------------------------
void ProcessService::NotifyError( const std::string& /*error*/, const std::string& endpoint )
{
    ::NotifyError( endpoint, server_, processes_ );
    NotifyStopped();
}

namespace
{
    template< typename T, typename U >
    void SendErrorMessage( LauncherPublisher& publisher, const tools::Path& exercise, const tools::Path& session, U error )
    {
        T message;
        message().set_exercise( exercise.ToUTF8() );
        message().set_session( session.ToUTF8() );
        message().set_error_code( error );
        message.Send( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::Update
// Created: AHC 2010-05-19
// -----------------------------------------------------------------------------
void ProcessService::Update()
{
    for( ProcessContainer::iterator it = processes_.begin(); processes_.end() != it; ++it )
        it->second->Update();
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendSessionsStatuses
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
void ProcessService::SendSessionsStatuses( const std::string& endpoint )
{
    if( endpoint.empty() )
        return;
    LauncherPublisher& publisher = server_.ResolveClient( endpoint );
    for( ProcessContainer::iterator cur = processes_.begin(); cur != processes_.end(); ++cur )
    {
        const auto spawns = cur->second->GetProcess()->GetSpawns();
        for( auto it = spawns.begin(); it != spawns.end(); ++it )
        {
            auto cmd = *it;
            if( cmd->GetName() != endpoint )
                continue;
            SessionStatus msg;
            const tools::Path exercise = cmd->GetExercise();
            const tools::Path session = cmd->GetSession();
            msg().set_status( IsRunning( exercise, session )? sword::SessionStatus::running : sword::SessionStatus::not_running );
            msg().set_exercise( exercise.ToUTF8() );
            msg().set_session( session.ToUTF8() );
            msg.Send( publisher );
        }
    }
}
