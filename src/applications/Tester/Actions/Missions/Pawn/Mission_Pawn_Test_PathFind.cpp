// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
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
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_Test_PathFind";
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


    ASN_Tools::CopyNumeric( pTarget_->GetTestParam_Numeric(), asnMission.nb_pathfind );


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


