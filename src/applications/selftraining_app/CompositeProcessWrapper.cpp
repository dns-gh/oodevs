// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "CompositeProcessWrapper.h"
#include "frontend/ProcessObserver_ABC.h"
#include "frontend/SpawnCommand.h"
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>

// warning C4127: conditional expression is constant
#pragma warning( disable: 4127 )

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper constructor
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
CompositeProcessWrapper::CompositeProcessWrapper( frontend::ProcessObserver_ABC& observer )
    : observer_( observer )
    , mutex_   ( new boost::mutex() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper destructor
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
CompositeProcessWrapper::~CompositeProcessWrapper()
{
    /// prevent bad mojo when this is destroyed before thread_
    thread_->join();
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::Add
// Created: BAX 2013-04-15
// -----------------------------------------------------------------------------
void CompositeProcessWrapper::Add( const T_Spawn& spawn )
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    spawns_.push_back( spawn );
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::GetSpawns
// Created: BAX 2013-04-15
// -----------------------------------------------------------------------------
T_Spawns CompositeProcessWrapper::GetSpawns() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    return spawns_;
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::Apply
// Created: BAX 2013-04-15
// -----------------------------------------------------------------------------
void CompositeProcessWrapper::Apply( const boost::function< void( frontend::SpawnCommand& ) >& operand )
{
    const T_Spawns spawns = GetSpawns();
    for( auto it = spawns.begin(); it != spawns.end(); ++it )
    {
        SetCurrent( *it );
        operand( **it );
    }
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::Apply
// Created: BAX 2013-04-15
// -----------------------------------------------------------------------------
void CompositeProcessWrapper::SetCurrent( const T_Spawn& spawn )
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    current_ = spawn;
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::Start
// Created: SBO 2010-11-09
// -----------------------------------------------------------------------------
void CompositeProcessWrapper::Start()
{
    for( auto it = spawns_.begin(); it != spawns_.end(); ++it )
        ( *it )->Attach( shared_from_this() );
    thread_.reset( new boost::thread( boost::bind( &CompositeProcessWrapper::Run, this ) ) );
}

namespace
{
    void WaitAny( const T_Spawns& spawns )
    {
        while( true )
            for( auto it = spawns.begin(); it != spawns.end(); ++it )
                if( !(*it)->Wait() )
                    return;
    }

    void Wait( frontend::SpawnCommand& spawn )
    {
        while( spawn.Wait() )
            continue;
    }
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::Run
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
void CompositeProcessWrapper::Run()
{
    try
    {
        Apply( boost::bind( &frontend::SpawnCommand::Start, _1 ) );
        WaitAny( GetSpawns() );
        boost::lock_guard< boost::mutex > lock( *mutex_ );
        current_.reset();
        spawns_.clear();
        observer_.NotifyStopped();
    }
    catch( const std::exception& e )
    {
        Stop();
        observer_.NotifyError( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::Stop
// Created: SBO 2010-11-09
// -----------------------------------------------------------------------------
void CompositeProcessWrapper::Stop()
{
    Apply( boost::bind( &frontend::SpawnCommand::Stop, _1 ) );
    Apply( boost::bind( &Wait, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::GetPercentage
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
unsigned int CompositeProcessWrapper::GetPercentage() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    unsigned sum = 0;
    for( auto it = spawns_.begin(); it != spawns_.end(); ++it )
            sum += (*it)->GetPercentage();
    return spawns_.empty() ? 0 : sum / static_cast< unsigned int >( spawns_.size() );
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::GetStatus
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
QString CompositeProcessWrapper::GetStatus() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    return current_ ? current_->GetStatus() : "";
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::GetStartedExercise
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
tools::Path CompositeProcessWrapper::GetStartedExercise() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    if( spawns_.empty() )
        return tools::Path();
    return spawns_.front()->GetStartedExercise();
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::GetExercise
// Created: RPD 2011-09-13
// -----------------------------------------------------------------------------
tools::Path CompositeProcessWrapper::GetExercise() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    if( spawns_.empty() )
        return tools::Path();
    return spawns_.front()->GetExercise();
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::GetSession
// Created: RPD 2011-09-13
// -----------------------------------------------------------------------------
tools::Path CompositeProcessWrapper::GetSession() const
{
    boost::lock_guard< boost::mutex > lock( *mutex_ );
    if( spawns_.empty() )
        return tools::Path();
    return spawns_.front()->GetSession();
}
