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

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Scheduler constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Scheduler::Scheduler()
    : actions_            ()
    , itCurrentAction_    ( actions_.begin() )
    , nNextExecutionTick_ ( 0 )
    , nExecutionStep_     ( 10 )
    , nTestRun_           ( 0 )
    , nTestSetRun_        ( 0 )
    , nTestTotal_         ( 0 )
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
// Name: Scheduler::Process
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
bool Scheduler::Run( uint nCurrentTick /* = 0 */ )
{
    if( itCurrentAction_ == actions_.end() )
    {
        nTestRun_ = 0;
        ++nTestSetRun_;
        itCurrentAction_ = actions_.begin();
        nStartTick_ -= nCurrentTick;
    }

    while( itCurrentAction_ != actions_.end()  && 
           itCurrentAction_->first <= nStartTick_ + nCurrentTick &&
           itCurrentAction_->second->IsReady() )
    {
        MT_LOG_INFO_MSG( "[" << nTestSetRun_ << "][" << nTestRun_ << "/" << nTestTotal_ << "] Starting action: " 
                             << itCurrentAction_->second->GetName() );
        itCurrentAction_->second->Run();
        ++itCurrentAction_;
        ++nTestRun_;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::AddAction
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Scheduler::AddAction( Action_ABC& action, int nExecutionTick /* = -1 */ )
{
    if( nExecutionTick == -1 )
        nExecutionTick = GetNextExecutionTick();
    actions_.insert( std::make_pair( nExecutionTick, &action ) );
    itCurrentAction_ = actions_.begin();
    ++nTestTotal_;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::GetNextExecutionTick
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
uint Scheduler::GetNextExecutionTick()
{
    uint nExecutionTick = nNextExecutionTick_;
    nNextExecutionTick_ += nExecutionStep_;
    return nExecutionTick;
}

// -----------------------------------------------------------------------------
// Name: Scheduler::SetStartTick
// Created: SBO 2005-08-22
// -----------------------------------------------------------------------------
void Scheduler::SetStartTick( uint nTick )
{
    nStartTick_ = nTick;
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
// Name: Scheduler::SetExecutionStep
// Created: SBO 2005-08-22
// -----------------------------------------------------------------------------
void Scheduler::SetExecutionStep( uint nStep )
{
    nExecutionStep_ = nStep;
}
