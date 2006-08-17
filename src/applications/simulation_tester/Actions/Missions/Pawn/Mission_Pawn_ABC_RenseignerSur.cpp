// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ABC_RenseignerSur.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_RenseignerSur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_RenseignerSur::Mission_Pawn_ABC_RenseignerSur( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_RenseignerSur", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_RenseignerSur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_RenseignerSur::~Mission_Pawn_ABC_RenseignerSur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_RenseignerSur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_RenseignerSur::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_ABC_RenseignerSur();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_renseigner_sur;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_renseigner_sur = &asnMission;

    const Position& pointARenseigner_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointARenseigner_, asnMission.point_a_renseigner );

    delete &pointARenseigner_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_RenseignerSur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_RenseignerSur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_renseigner_sur );
    ASN1T_Mission_Pion_ABC_RenseignerSur& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_renseigner_sur;

    ASN_Tools::Delete( asnMission.point_a_renseigner );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


