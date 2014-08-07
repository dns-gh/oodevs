// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ProcessWrapper.h"
#include "ProcessObserver_ABC.h"
#include "SpawnCommand.h"
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>

// warning C4127: conditional expression is constant
#pragma warning( disable: 4127 )

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: ProcessWrapper constructor
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
ProcessWrapper::ProcessWrapper( ProcessObserver_ABC& observer )
    : observer_( observer )
    , mutex_   ( new boost::mutex() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper destructor
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
ProcessWrapper::~ProcessWrapper()
{
    /// prevent bad mojo when this is destroyed before thread_
    Join();
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Add
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
void ProcessWrapper::Add( const T_Spawn& spawn )
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    spawns_.push_back( spawn );
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetSpawns
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
ProcessWrapper::T_Spawns ProcessWrapper::GetSpawns() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    return spawns_;
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Apply
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
void ProcessWrapper::Apply( const std::function< void( SpawnCommand& ) >& operand )
{
    const T_Spawns spawns = GetSpawns();
    for( auto it = spawns.begin(); it != spawns.end(); ++it )
    {
        SetCurrent( *it );
        operand( **it );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Apply
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
void ProcessWrapper::SetCurrent( const T_Spawn& spawn )
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    current_ = spawn;
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Start
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
void ProcessWrapper::Start( const boost::shared_ptr< ProcessWrapper >& process )
{
    const auto& spawns = process->spawns_;
    for( auto it = spawns.begin(); it != spawns.end(); ++it )
        ( *it )->Attach( process );
    process->thread_.reset( new boost::thread( boost::bind( &ProcessWrapper::Run, process.get() ) ) );
}

namespace
{
    void WaitAny( const ProcessWrapper::T_Spawns& spawns )
    {
        while( true )
            for( auto it = spawns.begin(); it != spawns.end(); ++it )
                if( (*it)->Wait( boost::posix_time::milliseconds( 100 ) ) )
                    return;
    }

    void Wait( SpawnCommand& spawn )
    {
        while( !spawn.Wait( boost::posix_time::milliseconds( 100 ) ) )
            continue;
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Run
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
void ProcessWrapper::Run()
{
    try
    {
        Apply( boost::bind( &SpawnCommand::Start, _1 ) );
        WaitAny( GetSpawns() );
        boost::lock_guard< boost::mutex > lock( *mutex_ );
        current_.reset();
        spawns_.clear();
        observer_.NotifyStopped();
    }
    catch( const std::exception& e )
    {
        Stop();
        boost::mutex::scoped_lock lock( *mutex_ );
        const std::string name = spawns_.empty()
                               ? std::string()
                               : spawns_.front()->GetName();
        lock.unlock();
        observer_.NotifyError( tools::GetExceptionMsg( e ), name );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::IsRunning
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
bool ProcessWrapper::IsRunning() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    return !spawns_.empty();
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Join
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
void ProcessWrapper::Join() const
{
    if( thread_.get() )
        thread_->join();
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Stop
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
void ProcessWrapper::Stop()
{
    Apply( boost::bind( &SpawnCommand::Stop, _1 ) );
    Apply( boost::bind( &Wait, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetPercentage
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
unsigned int ProcessWrapper::GetPercentage() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    unsigned sum = 0;
    for( auto it = spawns_.begin(); it != spawns_.end(); ++it )
            sum += (*it)->GetPercentage();
    return spawns_.empty() ? 0 : sum / static_cast< unsigned int >( spawns_.size() );
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetStatus
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
QString ProcessWrapper::GetStatus() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    return current_ ? current_->GetStatus() : "";
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetStartedExercise
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
tools::Path ProcessWrapper::GetStartedExercise() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    if( spawns_.empty() )
        return tools::Path();
    return spawns_.front()->GetStartedExercise();
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetExercise
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
tools::Path ProcessWrapper::GetExercise() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    if( spawns_.empty() )
        return tools::Path();
    return spawns_.front()->GetExercise();
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetSession
// Created: BAX 2013-04-18
// -----------------------------------------------------------------------------
tools::Path ProcessWrapper::GetSession() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    if( spawns_.empty() )
        return tools::Path();
    return spawns_.front()->GetSession();
}
