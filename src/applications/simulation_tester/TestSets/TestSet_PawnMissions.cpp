// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-11 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "TestSet_PawnMissions.h"
#include "Workspace.h"
#include "Actions/Scheduler.h"
#include "Actions/MagicActions/Action_Magic_Move.h"
#include "Actions/Missions/Mission_Pawn_Type.h"
#include "Entities/Pawn.h"
#include "Types/Entities/PawnModel.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: TestSet_PawnMissions constructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
TestSet_PawnMissions::TestSet_PawnMissions()
    : TestSet_ABC ()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: TestSet_PawnMissions destructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
TestSet_PawnMissions::~TestSet_PawnMissions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TestSet_PawnMissions::Load
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void TestSet_PawnMissions::Load( Workspace& workspace )
{
    Scheduler& scheduler = workspace.GetScheduler();

    Pawn* pPawn = workspace.GetEntityManager().FindPawn( 6000044 );
    if( pPawn )
        pPawn->ScheduleAllMissions( scheduler );
    else
        MT_LOG_ERROR_MSG( "Pawn does not exist" );
}

