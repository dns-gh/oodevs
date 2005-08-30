// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
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
    , nLastExecutionTick_   ( 0 )
    , nExecutionStep_       ( config.GetPeriod() )
    , nTestRun_             ( 0 )
    , nTestTotal_           ( 0 )
    , nSameMissionInterval_ ( config.GetIterationInterval() )
    , nRecoveryTick_        ( config.MustRecover() ? config.GetRecoveryTick() : 0 )
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
void Scheduler::RecoverIfNeeded( uint nCurrentTick )
{
    if( nRecoveryTick_ == 0 )
        return;
    if( itCurrentAction_ == actions_.end() )
        return;
    nCurrentTick_ = nRecoveryTick_;
    for( ; itCurrentAction_ != actions_.end() && itCurrentAction_->first < nCurrentTick_; ++itCurrentAction_ )
        ;
    nLastExecutionTick_ = nCurrentTick;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::Process
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
bool Scheduler::Run( uint nCurrentTick )
{
    if( itCurrentAction_ == actions_.end() )
    {
        itCurrentAction_ = actions_.begin();
        nCurrentTick_ = 0;
    }
    ++nCurrentTick_;
    while( itCurrentAction_ != actions_.end()  && 
           itCurrentAction_->first < nCurrentTick_ &&
           itCurrentAction_->second->IsReady() )
    {
        MT_LOG_INFO_MSG( "[" << nTestRun_ / nTestTotal_ << "][" << nTestRun_ % nTestTotal_ << "/" << nTestTotal_ 
                             << "] Starting action: " << itCurrentAction_->second->GetName() );
        itCurrentAction_->second->Run();
        ++itCurrentAction_;
        ++nTestRun_;
    }
    nLastExecutionTick_ = nCurrentTick;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::AddAction
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Scheduler::AddAction( Action_ABC& action, uint nExecutionTick )
{
    actions_.insert( std::make_pair( nExecutionTick, &action ) );
    itCurrentAction_ = actions_.begin();
    ++nTestTotal_;
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
    nNextExecutionTick_ += nExecutionStep_;    
}

// -----------------------------------------------------------------------------
// Name: Scheduler::AddActions
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
void Scheduler::AddActions( Action_ABC& action, uint nIteration )
{
    for( uint i = 0; i < nIteration; ++i )
    {
        actions_.insert( std::make_pair( nNextExecutionTick_ + i * nSameMissionInterval_, &action ) );
        itCurrentAction_ = actions_.begin();
        ++nTestTotal_;
    }
    nNextExecutionTick_ += nExecutionStep_;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::GetNextExecutionTick
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
uint Scheduler::GetNextExecutionTick() const
{
    return nLastExecutionTick_ + nExecutionStep_;
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
uint Scheduler::GetCurrentTick() const
{
    return nCurrentTick_;
}
