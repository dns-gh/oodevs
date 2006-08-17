// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_InterdireFranchissementPopulations.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_InterdireFranchissementPopulations constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_InterdireFranchissementPopulations::Mission_Pawn_InterdireFranchissementPopulations( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_InterdireFranchissementPopulations", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_InterdireFranchissementPopulations destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_InterdireFranchissementPopulations::~Mission_Pawn_InterdireFranchissementPopulations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_InterdireFranchissementPopulations::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_InterdireFranchissementPopulations::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_InterdireFranchissementPopulations& asnMission = *new ASN1T_Mission_Pion_InterdireFranchissementPopulations();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_interdire_franchissement_populations;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_interdire_franchissement_populations = &asnMission;

    const Position& point_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( point_, asnMission.point );

    delete &point_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_InterdireFranchissementPopulations::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_InterdireFranchissementPopulations::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_interdire_franchissement_populations );
    ASN1T_Mission_Pion_InterdireFranchissementPopulations& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_interdire_franchissement_populations;

    ASN_Tools::Delete( asnMission.point );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


