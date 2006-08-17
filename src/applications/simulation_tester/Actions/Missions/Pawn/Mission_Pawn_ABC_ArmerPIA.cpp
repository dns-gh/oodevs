// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ABC_ArmerPIA.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ArmerPIA constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ArmerPIA::Mission_Pawn_ABC_ArmerPIA( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_ArmerPIA", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ArmerPIA destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ArmerPIA::~Mission_Pawn_ABC_ArmerPIA()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ArmerPIA::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ArmerPIA::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_ArmerPIA& asnMission = *new ASN1T_Mission_Pion_ABC_ArmerPIA();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_armer_pia;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_armer_pia = &asnMission;

    const Position& pia_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pia_, asnMission.pia );

    delete &pia_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ArmerPIA::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ArmerPIA::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_armer_pia );
    ASN1T_Mission_Pion_ABC_ArmerPIA& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_armer_pia;

    ASN_Tools::Delete( asnMission.pia );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


