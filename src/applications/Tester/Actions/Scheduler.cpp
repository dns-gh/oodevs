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

#include "Actions/Action_ABC.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Scheduler constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Scheduler::Scheduler()
    : actions_         ()
    , itCurrentAction_ ( actions_.begin() )
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
bool Scheduler::Run()
{
    if( itCurrentAction_ == actions_.end() )
        return false;

    if( itCurrentAction_->second->IsReady() )
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
void Scheduler::AddAction( Action_ABC& action )
{
    bool bOut = actions_.insert( std::make_pair( action.GetExecutionTick(), &action ) ).second;
    assert( bOut );
    itCurrentAction_ = actions_.begin();
}
