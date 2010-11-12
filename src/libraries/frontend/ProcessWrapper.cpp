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
#pragma warning( push )
#pragma warning( disable : 4127 4244 4511 4512 )
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#pragma warning( pop )

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
    observer_.ProcessStopped();
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
        process_->Start();
        while( process_->Wait() ) {}
        process_.reset();
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
        if( thread_.get() )
            thread_->join();
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
