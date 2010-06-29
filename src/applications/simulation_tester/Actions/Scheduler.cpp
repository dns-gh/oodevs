// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Scheduler.h"
#include "Action_ABC.h"
#include "Config.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Scheduler constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Scheduler::Scheduler( const Config& config )
    : actions_              ()
    , itCurrentAction_      ( actions_.begin() )
    , nNextExecutionTick_   ( 0 )
    , nCurrentTick_         ( 0 )
    , nExecutionPeriod_     ( config.GetPeriod() )
    , nRecompletionPeriod_  ( config.GetRecompletionPeriod() )
    , nSameMissionInterval_ ( config.GetIterationInterval() )
    , nRecoveryTick_        ( config.MustRecover() ? config.GetRecoveryTick() : 0 )
    , nLastExecutionTick_   ( 0 )
    , nMissionLimitCounter_ ( 0 )
    , nMissionInPeriod_     ( 0 )
    , nMaxMissionInPeriod_  ( config.GetMaxMissionPerTick() )
    , nTestRun_             ( 0 )
    , nTestTotal_           ( 0 )
    , nSimTicks_            ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Scheduler destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Scheduler::~Scheduler()
{
    actions_.clear();
}

// -----------------------------------------------------------------------------
// Name: Scheduler::RecoverIfNeeded
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
void Scheduler::RecoverIfNeeded( unsigned int nCurrentTick )
{
    if( nRecoveryTick_ == 0 )
        return;
    if( itCurrentAction_ == actions_.end() )
        return;
    MT_LOG_INFO_MSG( "Restarting at " << nRecoveryTick_ );
    nCurrentTick_ = ( nRecoveryTick_ / nExecutionPeriod_ ) * nExecutionPeriod_ - 1;
    // pass all the actions already executed
    for( ; itCurrentAction_ != actions_.end() && (int)itCurrentAction_->first < nCurrentTick_; ++itCurrentAction_ )
    {
        MT_LOG_INFO_MSG( "test already passed: " << itCurrentAction_->second->ToString() );
        ++nTestRun_;
    }
    nLastExecutionTick_ = nCurrentTick;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::Process
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
bool Scheduler::Run( unsigned int nCurrentTick )
{
    nSimTicks_ = nCurrentTick;
    if( itCurrentAction_ == actions_.end() )
    {
        itCurrentAction_ = actions_.begin();
        nCurrentTick_    = ( -1 ) * ( int )nExecutionPeriod_;
    }
    ++nCurrentTick_;
    if( nMissionLimitCounter_ )
    {
        --nMissionLimitCounter_;
        return true;
    }

    while( itCurrentAction_ != actions_.end()           &&
           (int)itCurrentAction_->first < nCurrentTick_ &&
           nMissionInPeriod_ < nMaxMissionInPeriod_ )
    {
        MT_LOG_INFO_MSG( "[" << nTestRun_ / nTestTotal_ << "][" << nTestRun_ % nTestTotal_ << "/" << nTestTotal_
                             << "] Starting action: " << itCurrentAction_->second->ToString() );
        itCurrentAction_->second->Run( *this );
        ++itCurrentAction_;
        ++nTestRun_;
        nLastExecutionTick_ = nCurrentTick;
        ++nMissionInPeriod_;
    }
    if( nMissionInPeriod_ == nMaxMissionInPeriod_ )
    {
        nMissionLimitCounter_ = nExecutionPeriod_ - 1;
        nMissionInPeriod_ = 0;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::AddAction
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
void Scheduler::AddAction( Action_ABC& action )
{
    actions_.insert( std::make_pair( nNextExecutionTick_, &action ) );
    itCurrentAction_ = actions_.begin();
    ++nTestTotal_;
    nNextExecutionTick_ += nExecutionPeriod_;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::AddActions
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
void Scheduler::AddActions( Action_ABC& action, unsigned int nIteration )
{
    for( unsigned int i = 0; i < nIteration; ++i )
    {
        actions_.insert( std::make_pair( nNextExecutionTick_ + i * nSameMissionInterval_, &action ) );
        itCurrentAction_ = actions_.begin();
        ++nTestTotal_;
    }
    nNextExecutionTick_ += nExecutionPeriod_;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::GetNextExecutionTick
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
unsigned int Scheduler::GetNextExecutionTick() const
{
    return nLastExecutionTick_ + nExecutionPeriod_;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::ResetExecutionTick
// Created: SBO 2005-08-22
// -----------------------------------------------------------------------------
void Scheduler::ResetExecutionTick()
{
    nNextExecutionTick_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::GetCurrentTick
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
unsigned int Scheduler::GetCurrentTick() const
{
    return nCurrentTick_ > 0 ? nCurrentTick_ : 0;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::MustRecomplete
// Created: SBO 2005-09-01
// -----------------------------------------------------------------------------
bool Scheduler::MustRecomplete( unsigned int& lastRecompletion ) const
{
    if( nRecompletionPeriod_ <= 0 )
        return false;
    if( nSimTicks_ > lastRecompletion + nRecompletionPeriod_ )
    {
        lastRecompletion = nSimTicks_;
        return true;
    }
    return false;
}
