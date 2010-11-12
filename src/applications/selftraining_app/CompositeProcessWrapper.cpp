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
#pragma warning( push )
#pragma warning( disable : 4127 4244 4511 4512 )
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper constructor
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
CompositeProcessWrapper::CompositeProcessWrapper( frontend::ProcessObserver_ABC& observer, boost::shared_ptr< frontend::SpawnCommand > process1, boost::shared_ptr< frontend::SpawnCommand > process2 )
    : observer_( observer )
    , first_( process1 )
    , second_( process2 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper destructor
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
CompositeProcessWrapper::~CompositeProcessWrapper()
{
    observer_.ProcessStopped();
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::Start
// Created: SBO 2010-11-09
// -----------------------------------------------------------------------------
void CompositeProcessWrapper::Start()
{
    if( first_.get() )
        first_->Attach( shared_from_this() );
    if( second_.get() )
        second_->Attach( shared_from_this() );
    thread_.reset( new boost::thread( boost::bind( &CompositeProcessWrapper::Run, this ) ) );
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::Run
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
void CompositeProcessWrapper::Run()
{
    if( first_.get() && second_.get() )
    {
        current_ = first_;
        first_->Start();
        current_ = second_;
        second_->Start();
        while( first_->Wait() && second_->Wait() ) {}
        current_.reset();
        first_.reset();
        second_.reset();
    }
}

// -----------------------------------------------------------------------------
// Name: CompositeProcessWrapper::Stop
// Created: SBO 2010-11-09
// -----------------------------------------------------------------------------
void CompositeProcessWrapper::Stop()
{
    if( first_.get() )
    {
        first_->Stop();
        if( thread_.get() )
            thread_->join();
    }
    if( second_.get() )
    {
        second_->Stop();
        if( thread_.get() )
            thread_->join();
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
