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
#include "Mission_Pawn_Test_CreateBypass.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_CreateBypass constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_CreateBypass::Mission_Pawn_Test_CreateBypass( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_Test_CreateBypass";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_CreateBypass destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_CreateBypass::~Mission_Pawn_Test_CreateBypass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_CreateBypass::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_CreateBypass::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_CreateBypass& asnMission = *new ASN1T_Mission_Pion_Test_CreateBypass();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_create_bypass;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_create_bypass = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.objet );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_CreateBypass::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_CreateBypass::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_test_create_bypass );
    ASN1T_Mission_Pion_Test_CreateBypass& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_test_create_bypass;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


