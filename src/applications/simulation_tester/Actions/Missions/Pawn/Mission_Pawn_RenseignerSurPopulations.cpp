// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_RenseignerSurPopulations.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RenseignerSurPopulations constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RenseignerSurPopulations::Mission_Pawn_RenseignerSurPopulations( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_RenseignerSurPopulations", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RenseignerSurPopulations destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RenseignerSurPopulations::~Mission_Pawn_RenseignerSurPopulations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RenseignerSurPopulations::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RenseignerSurPopulations::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_RenseignerSurPopulations& asnMission = *new ASN1T_Mission_Pion_RenseignerSurPopulations();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_renseigner_sur_populations;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_renseigner_sur_populations = &asnMission;

    const Position& point_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( point_, asnMission.point );

    delete &point_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RenseignerSurPopulations::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RenseignerSurPopulations::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_renseigner_sur_populations );
    ASN1T_Mission_Pion_RenseignerSurPopulations& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_renseigner_sur_populations;

    ASN_Tools::Delete( asnMission.point );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


