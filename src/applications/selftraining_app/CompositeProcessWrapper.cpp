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
#include "clients_kernel/Controller.h"
#include "frontend/SpawnCommand.h"
#pragma warning( push )
#pragma warning( disable : 4127 4244 4511 4512 )
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper constructor
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
CompositeProcessWrapper::CompositeProcessWrapper( kernel::Controller& controller, boost::shared_ptr< frontend::SpawnCommand > process1, boost::shared_ptr< frontend::SpawnCommand > process2 )
    : controller_( controller )
    , current_( process1 )
    , first_( process1 )
    , second_( process2 )
    , thread_( new boost::thread( boost::bind( &CompositeProcessWrapper::ThreadStart, this ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper destructor
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
CompositeProcessWrapper::~CompositeProcessWrapper()
{
    try
    {
        if( thread_.get() )
        {
            thread_->interrupt();
            current_.reset();
            first_.reset();
            second_.reset();
            thread_->join();
        }
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::ThreadStart
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
void CompositeProcessWrapper::ThreadStart()
{
    try
    {
        if( first_.get() )
        {
            controller_.Update( shared_from_this() );
            boost::this_thread::interruption_point();
            first_->Start();
            boost::this_thread::interruption_point();
            first_->Wait();
            boost::this_thread::interruption_point();
            controller_.Update( shared_from_this() );
            boost::this_thread::interruption_point();
        }
        if( second_.get() )
        {
            current_ = second_;
            second_->Start();
            boost::this_thread::interruption_point();
            controller_.Update( shared_from_this() );
            boost::this_thread::interruption_point();
            second_->Wait();
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
// Name: CompositeProcessWrapper::GetPercentage
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
unsigned int CompositeProcessWrapper::GetPercentage() const
{
    return first_.get() && second_.get() ? ( first_->GetPercentage() + second_->GetPercentage() ) / 2 : 0;
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::GetStatus
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
QString CompositeProcessWrapper::GetStatus() const
{
    return current_.get() ? current_->GetStatus() : "";
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::GetStartedExercise
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
std::string CompositeProcessWrapper::GetStartedExercise() const
{
    return first_.get() ? first_->GetStartedExercise() : std::string();
}
