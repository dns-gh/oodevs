// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-17 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "TestSet_AllAutomatMissions.h"
#include "Workspace.h"
#include "Actions/Scheduler.h"
#include "Actions/Missions/Mission_Automat_Type.h"
#include "Entities/Automat.h"
#include "Types/Entities/AutomatModel.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: TestSet_AllAutomatMissions constructor
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
TestSet_AllAutomatMissions::TestSet_AllAutomatMissions( unsigned int nIteration /* = 1 */ )
    : TestSet_ABC ( nIteration )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TestSet_AllAutomatMissions destructor
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
TestSet_AllAutomatMissions::~TestSet_AllAutomatMissions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TestSet_AllAutomatMissions::Load
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void TestSet_AllAutomatMissions::Load( Workspace& workspace )
{
    Scheduler& scheduler = workspace.GetScheduler();
    // launch all missions in parallel
    workspace.GetEntityManager().ScheduleAllAutomatMissions( scheduler, true, nIteration_ );
}
