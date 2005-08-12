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
#include "Mission_Pawn_Test_ChangePosture.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_ChangePosture constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_ChangePosture::Mission_Pawn_Test_ChangePosture( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_Test_ChangePosture";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_ChangePosture destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_ChangePosture::~Mission_Pawn_Test_ChangePosture()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_ChangePosture::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_ChangePosture::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_ChangePosture& asnMission = *new ASN1T_Mission_Pion_Test_ChangePosture();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_change_posture;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_change_posture = &asnMission;

    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 6 ), asnMission.posture );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_ChangePosture::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_ChangePosture::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_test_change_posture );
    ASN1T_Mission_Pion_Test_ChangePosture& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_test_change_posture;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


