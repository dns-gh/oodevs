// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_Test_Reinforce.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Reinforce constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_Reinforce::Mission_Pawn_Test_Reinforce( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Test_Reinforce", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Reinforce destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_Reinforce::~Mission_Pawn_Test_Reinforce()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Reinforce::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_Reinforce::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_Reinforce& asnMission = *new ASN1T_Mission_Pion_Test_Reinforce();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_reinforce;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_reinforce = &asnMission;


    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.unite );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.renforce );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Reinforce::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_Reinforce::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_test_reinforce );
    ASN1T_Mission_Pion_Test_Reinforce& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_test_reinforce;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


