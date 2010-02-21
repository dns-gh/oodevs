// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
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
    : Mission_Pawn_ABC ( "Pawn_Test_CreateBypass", pawn )
{
    // NOTHING
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

    Mission_Pion_Test_CreateBypass& asnMission = *new Mission_Pion_Test_CreateBypass();

    asnMsg_.GetAsnMsg().mission().mission_pion_test_create_bypass = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.objet );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_CreateBypass::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_CreateBypass::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_test_create_bypass ());
    Mission_Pion_Test_CreateBypass& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_test_create_bypass;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


