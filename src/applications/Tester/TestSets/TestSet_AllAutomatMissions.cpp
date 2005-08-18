// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
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
TestSet_AllAutomatMissions::TestSet_AllAutomatMissions()
    : TestSet_ABC ()
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
    workspace.GetEntityManager().ScheduleAllAutomatMissions( scheduler );
}
