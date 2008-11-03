// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ProcessWrapper.h"
#include "clients_kernel/Controller.h"
#include "frontend/SpawnCommand.h"
#pragma warning( push )
#pragma warning( disable : 4127 4244 4511 4512 )
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: ProcessWrapper constructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
ProcessWrapper::ProcessWrapper( kernel::Controller& controller, boost::shared_ptr< frontend::SpawnCommand > process )
    : controller_( controller )
    , process_( process )
    , thread_( new boost::thread( boost::bind( &ProcessWrapper::ThreadStart, this ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper destructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
ProcessWrapper::~ProcessWrapper()
{
    try
    {
        if( thread_.get() )
        {
            thread_->interrupt();
            process_.reset();
            thread_->join();
        }
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessWrapper::ThreadStart
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void ProcessWrapper::ThreadStart()
{
    try
    {
        if( process_.get() )
        {
            controller_.Update( shared_from_this() );
            boost::this_thread::interruption_point();
            process_->Start();
            boost::this_thread::interruption_point();
            process_->Wait();
            boost::this_thread::interruption_point();
            controller_.Delete( shared_from_this() );
            boost::this_thread::interruption_point();
        }
    }
    catch( ... )
    {
        // NOTHING
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
