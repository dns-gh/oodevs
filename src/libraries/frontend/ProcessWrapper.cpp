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
    thread_.reset( new boost::thread( boost::bind( &ProcessWrapper::Run, this ) ) );
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::Run
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void ProcessWrapper::Run()
{
    if( process_.get() )
    {
        try
        {
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
void ProcessWrapper::Stop()
{
    if( process_.get() )
    {
        process_->Stop();
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
// Name: ProcessWrapper::GetStartedExercise
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
std::string ProcessWrapper::GetStartedExercise() const
{
    return process_->GetStartedExercise();
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetExercise
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
std::string ProcessWrapper::GetExercise() const
{
    return process_->GetExercise();
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetSession
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
std::string ProcessWrapper::GetSession() const
{
    return process_->GetSession();
}


// -----------------------------------------------------------------------------
// Name: ProcessWrapper::GetProcess
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
const SpawnCommand* ProcessWrapper::GetCommand() const
{
    return process_.get();
}