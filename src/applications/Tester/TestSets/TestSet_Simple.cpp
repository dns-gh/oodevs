// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-05 $
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
#include "TestSet_Simple.h"
#include "TestManager.h"
#include "Actions/Scheduler.h"
#include "Actions/Actions/Action_Magic_Move.h"
#include "Actions/Actions/Missions/Pawn/Mission_Pawn_MoveTo.h"
#include "Actions/Actions/Missions/Pawn/Mission_Pawn_Heliporter.h"
#include "Actions/Actions/Missions/Pawn/Mission_Pawn_Fire.h"
#include "Entities/Pawn.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: TestSet_Simple constructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
TestSet_Simple::TestSet_Simple()
    : TestSet_ABC ()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: TestSet_Simple destructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
TestSet_Simple::~TestSet_Simple()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TestSet_Simple::Load
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void TestSet_Simple::Load( Scheduler& scheduler )
{
    assert( !pScheduler_ );
    pScheduler_ = &scheduler;

    Pawn* pPawn = Pawn::Find( 6000044 );
    if( pPawn )
    {
        Action_Magic_Move*          pMagicMove         = new Action_Magic_Move( *pPawn, 25 );
        pScheduler_->AddAction( *pMagicMove );

        Mission_Pawn_Heliporter*    pMissionHeliporter = new Mission_Pawn_Heliporter( *pPawn, 75 );
        pScheduler_->AddAction( *pMissionHeliporter );
    }
    else
        MT_LOG_ERROR_MSG( "TEST 1 : Pawn does not exist" );

    pPawn = Pawn::Find( 6000042 );
    if( pPawn )
    {
        Mission_Pawn_MoveTo* pMissionMoveTo = new Mission_Pawn_MoveTo( *pPawn, 50 );
        pScheduler_->AddAction( *pMissionMoveTo );

        Mission_Pawn_Fire*   pMissionFire   = new Mission_Pawn_Fire( *pPawn, 20 );
        pScheduler_->AddAction( *pMissionFire );
    }
    else
        MT_LOG_ERROR_MSG( "TEST 2 : Pawn does not exist" );

}
