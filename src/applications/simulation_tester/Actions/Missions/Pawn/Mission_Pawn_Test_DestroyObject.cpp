// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_Test_DestroyObject.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_DestroyObject constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_DestroyObject::Mission_Pawn_Test_DestroyObject( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Test_DestroyObject", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_DestroyObject destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_DestroyObject::~Mission_Pawn_Test_DestroyObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_DestroyObject::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_DestroyObject::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_DestroyObject& asnMission = *new ASN1T_Mission_Pion_Test_DestroyObject();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_destroy_object;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_destroy_object = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.objet );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_DestroyObject::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_DestroyObject::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_test_destroy_object );
    ASN1T_Mission_Pion_Test_DestroyObject& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_test_destroy_object;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


