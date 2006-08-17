// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_Test_PathFind.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_PathFind constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_PathFind::Mission_Pawn_Test_PathFind( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Test_PathFind", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_PathFind destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_PathFind::~Mission_Pawn_Test_PathFind()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_PathFind::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_PathFind::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_PathFind& asnMission = *new ASN1T_Mission_Pion_Test_PathFind();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_path_find;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_path_find = &asnMission;


    ASN_Tools::CopyNumeric( pTarget_->GetTestParam_Numeric( 0, 4294967295 ), asnMission.nb_pathfind );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_PathFind::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_PathFind::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_test_path_find );
    ASN1T_Mission_Pion_Test_PathFind& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_test_path_find;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


