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

#include "Tester_pch.h"
#include "TestSet_AllPopulationMissions.h"
#include "Workspace.h"
#include "Actions/Scheduler.h"
#include "Actions/Missions/Mission_Population_Type.h"
#include "Entities/Population.h"
#include "Entities/EntityManager.h"
#include "Types/Entities/PopulationModel.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: TestSet_AllPopulationMissions constructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
TestSet_AllPopulationMissions::TestSet_AllPopulationMissions( uint nIteration /* = 1 */ )
    : TestSet_ABC ( nIteration )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: TestSet_AllPopulationMissions destructor
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
TestSet_AllPopulationMissions::~TestSet_AllPopulationMissions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TestSet_AllPopulationMissions::Load
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void TestSet_AllPopulationMissions::Load( Workspace& workspace )
{
    Scheduler& scheduler = workspace.GetScheduler();
    workspace.GetEntityManager().ScheduleAllPopulationMissions( scheduler, true, nIteration_ );
}
