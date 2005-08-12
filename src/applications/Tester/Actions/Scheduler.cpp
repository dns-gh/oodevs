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
        return false;

    std::cout << nCurrentTick << " != " << itCurrentAction_->first << std::endl;
    while( itCurrentAction_ != actions_.end()  && 
           itCurrentAction_->first <= nCurrentTick &&
           itCurrentAction_->second->IsReady() )
    {
        itCurrentAction_->second->Run();
        ++itCurrentAction_;
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
    bool bOut = actions_.insert( std::make_pair( nExecutionTick, &action ) ).second;
    assert( bOut );
    itCurrentAction_ = actions_.begin();
}

// -----------------------------------------------------------------------------
// Name: Scheduler::GetNextExecutionTick
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
uint Scheduler::GetNextExecutionTick()
{
    uint nExecutionTick = nNextExecutionTick_;
    nNextExecutionTick_ += 10;
    return nExecutionTick;
}
