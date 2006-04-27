// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_GEN_ComblerFosseAC.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ComblerFosseAC constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ComblerFosseAC::Mission_Pawn_GEN_ComblerFosseAC( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_ComblerFosseAC", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ComblerFosseAC destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ComblerFosseAC::~Mission_Pawn_GEN_ComblerFosseAC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ComblerFosseAC::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ComblerFosseAC::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_ComblerFosseAC& asnMission = *new ASN1T_Mission_Pion_GEN_ComblerFosseAC();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_combler_fosse_ac;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_combler_fosse_ac = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.obstacle );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ComblerFosseAC::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ComblerFosseAC::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_combler_fosse_ac );
    ASN1T_Mission_Pion_GEN_ComblerFosseAC& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_combler_fosse_ac;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


