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

#include "simulation_tester_pch.h"
#include "TestSet_Simple.h"
#include "Workspace.h"
#include "Actions/Scheduler.h"
#include "Actions/MagicActions/Action_Magic_Move.h"
#include "Actions/Missions/Mission_Pawn_Type.h"
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
void TestSet_Simple::Load( Workspace& workspace )
{
    Scheduler& scheduler = workspace.GetScheduler();

    Pawn* pPawn = workspace.GetEntityManager().FindPawn( 6000044 );
    if( pPawn )
    {
        Action_Magic_Move* pMagicMove = new Action_Magic_Move( *pPawn );
        scheduler.AddAction( *pMagicMove );
        pPawn->ScheduleMission( scheduler, "Pion Test Heliporter" );
    }
    else
        MT_LOG_ERROR_MSG( "TEST 1 : Pawn does not exist" );

    pPawn = workspace.GetEntityManager().FindPawn( 6000042 );
    if( pPawn )
    {
        pPawn->ScheduleMission( scheduler, "Pion Test MoveTo" );
        pPawn->ScheduleMission( scheduler, "Pion Test Fire"   );
    }
    else
        MT_LOG_ERROR_MSG( "TEST 2 : Pawn does not exist" );

}
