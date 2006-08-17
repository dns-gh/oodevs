// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_CanaliserPopulations.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CanaliserPopulations constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_CanaliserPopulations::Mission_Pawn_CanaliserPopulations( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_CanaliserPopulations", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CanaliserPopulations destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_CanaliserPopulations::~Mission_Pawn_CanaliserPopulations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CanaliserPopulations::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_CanaliserPopulations::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_CanaliserPopulations& asnMission = *new ASN1T_Mission_Pion_CanaliserPopulations();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_canaliser_populations;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_canaliser_populations = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Location();

    ASN_Tools::CopyLocation( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CanaliserPopulations::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_CanaliserPopulations::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_canaliser_populations );
    ASN1T_Mission_Pion_CanaliserPopulations& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_canaliser_populations;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


