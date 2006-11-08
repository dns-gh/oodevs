// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_ReprendreTravaux.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReprendreTravaux constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ReprendreTravaux::Mission_Pawn_GEN_ReprendreTravaux( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_ReprendreTravaux", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReprendreTravaux destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ReprendreTravaux::~Mission_Pawn_GEN_ReprendreTravaux()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReprendreTravaux::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ReprendreTravaux::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_ReprendreTravaux& asnMission = *new ASN1T_Mission_Pion_GEN_ReprendreTravaux();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_reprendre_travaux;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_reprendre_travaux = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.travaux );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReprendreTravaux::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ReprendreTravaux::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_reprendre_travaux );
    ASN1T_Mission_Pion_GEN_ReprendreTravaux& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_reprendre_travaux;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


