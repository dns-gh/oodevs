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
#pragma warning( pop )
#include <boost/bind.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: ProcessWrapper constructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
ProcessWrapper::ProcessWrapper( ProcessObserver_ABC& observer, boost::shared_ptr< SpawnCommand > process )
    : observer_( observer )
    , process_( process )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper destructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
ProcessWrapper::~ProcessWrapper()
{
    observer_.NotifyStopped();
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Start
// Created: SBO 2010-11-10
// -----------------------------------------------------------------------------
void ProcessWrapper::Start()
{
    process_->Attach( shared_from_this() );
    thread_.reset( new boost::thread( boost::bind( &ProcessWrapper::Run, this, true ) ) );
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Attach
// Created: JSR 2011-12-14
// -----------------------------------------------------------------------------
void ProcessWrapper::Attach()
{
    process_->Attach( shared_from_this() );
    thread_.reset( new boost::thread( boost::bind( &ProcessWrapper::Run, this, false ) ) );
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Run
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void ProcessWrapper::Run( bool start )
{
    if( process_.get() )
    {
        try
        {
            if( start )
                process_->Start();
            while( process_->Wait() ) {}
            process_.reset();
        }
        catch( std::exception& e )
        {
            Stop();
            observer_.NotifyError( e.what(), process_->GetCommanderEndpoint() );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::StartAndBlockMainThread
// Created: ABR 2011-06-27
// -----------------------------------------------------------------------------
void ProcessWrapper::StartAndBlockMainThread()
{
    process_->Attach( shared_from_this() );
    thread_.reset( new boost::thread( boost::bind( &ProcessWrapper::RunBlockingMainThread, this ) ) );
    thread_->join();
    process_.reset();
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::RunBlockingMainThread
// Created: ABR 2011-06-27
// -----------------------------------------------------------------------------
void ProcessWrapper::RunBlockingMainThread()
{
    if( process_.get() )
    {
        try
        {
            process_->Start();
            while( process_->Wait() ) {}
        }
        catch( std::exception& e )
        {
            Stop();
            observer_.NotifyError( e.what(), process_->GetCommanderEndpoint() );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Stop
// Created: SBO 2010-11-10
// -----------------------------------------------------------------------------
void ProcessWrapper::Stop( bool forceProcessStop /*= true*/ )
{
    if( process_.get() )
    {
        process_->Stop( forceProcessStop);
        while( process_->Wait() ) {}
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetPercentage
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
unsigned int ProcessWrapper::GetPercentage() const
{
    return process_.get() ? process_->GetPercentage() : 0;
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetStatus
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
QString ProcessWrapper::GetStatus() const
{
    return process_.get() ? process_->GetStatus() : "";
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetProcess
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
const SpawnCommand* ProcessWrapper::GetCommand() const
{
    return process_.get();
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::SetEndpoint
// Created: JSR 2011-12-13
// -----------------------------------------------------------------------------
void ProcessWrapper::SetEndpoint( const std::string& endpoint )
{
    if( process_.get() )
        process_->SetCommanderEndpoint( endpoint );
}
