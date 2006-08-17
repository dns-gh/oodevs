// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_CreerAirePoser.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerAirePoser constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_CreerAirePoser::Mission_Pawn_GEN_CreerAirePoser( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_CreerAirePoser", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerAirePoser destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_CreerAirePoser::~Mission_Pawn_GEN_CreerAirePoser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerAirePoser::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_CreerAirePoser::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_CreerAirePoser& asnMission = *new ASN1T_Mission_Pion_GEN_CreerAirePoser();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_aire_poser;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_creer_aire_poser = &asnMission;

    const Position& posObstacle_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( posObstacle_, asnMission.pos_obstacle );
    ASN_Tools::CopyID( pTarget_->GetTestParam_ID(), asnMission.oid_obstacle_planifie );

    delete &posObstacle_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerAirePoser::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_CreerAirePoser::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_creer_aire_poser );
    ASN1T_Mission_Pion_GEN_CreerAirePoser& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_creer_aire_poser;

    ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


