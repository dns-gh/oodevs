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

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "TestSet_PawnMissions.h"
#include "TestManager.h"
#include "Actions/Scheduler.h"
#include "Actions/MagicActions/Action_Magic_Move.h"
#include "Actions/Missions/Mission_Pawn_Type.h"
#include "Entities/Pawn.h"
#include "Entities/Types/PawnModel.h"

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
void TestSet_PawnMissions::Load( Scheduler& scheduler )
{
    assert( !pScheduler_ );
    pScheduler_ = &scheduler;

    Pawn* pPawn = Pawn::Find( 6000044 );
    if( pPawn )
    {
        const T_MissionPawnTypeVector& missions = pPawn->GetType().GetModel().GetMissions();
    
        uint nExecTick = 20;
        for( CIT_MissionPawnTypeVector it = missions.begin(); it != missions.end(); ++it )
        {
            pScheduler_->AddAction( Mission_Pawn_Type::CreateMission( **it, *pPawn, nExecTick ) );
            nExecTick += 10;
        }
    }
    else
        MT_LOG_ERROR_MSG( "Pawn does not exist" );
}


