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
#include "TestSet_AllPawnMissions.h"
#include "Workspace.h"
#include "Actions/Scheduler.h"
#include "Actions/Missions/Mission_Pawn_Type.h"
#include "Entities/Pawn.h"
#include "Types/Entities/PawnModel.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: TestSet_AllPawnMissions constructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
TestSet_AllPawnMissions::TestSet_AllPawnMissions( uint nIteration /* = 1 */ )
    : TestSet_ABC ( nIteration )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: TestSet_AllPawnMissions destructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
TestSet_AllPawnMissions::~TestSet_AllPawnMissions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TestSet_AllPawnMissions::Load
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void TestSet_AllPawnMissions::Load( Workspace& workspace )
{
    Scheduler& scheduler = workspace.GetScheduler();
    workspace.GetEntityManager().ScheduleAllPawnMissions( scheduler, true, nIteration_ );
}
