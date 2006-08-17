// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_DegagerEboulement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerEboulement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DegagerEboulement::Mission_Pawn_GEN_DegagerEboulement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_DegagerEboulement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerEboulement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DegagerEboulement::~Mission_Pawn_GEN_DegagerEboulement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerEboulement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DegagerEboulement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_DegagerEboulement& asnMission = *new ASN1T_Mission_Pion_GEN_DegagerEboulement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_degager_eboulement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_degager_eboulement = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.obstacle );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerEboulement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DegagerEboulement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_degager_eboulement );
    ASN1T_Mission_Pion_GEN_DegagerEboulement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_degager_eboulement;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


