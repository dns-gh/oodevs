// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_CreerPlateForme.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerPlateForme constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_CreerPlateForme::Mission_Pawn_GEN_CreerPlateForme( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_CreerPlateForme", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerPlateForme destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_CreerPlateForme::~Mission_Pawn_GEN_CreerPlateForme()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerPlateForme::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_CreerPlateForme::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_CreerPlateForme& asnMission = *new ASN1T_Mission_Pion_GEN_CreerPlateForme();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_plate_forme;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_creer_plate_forme = &asnMission;

    const Position& posObstacle_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( posObstacle_, asnMission.pos_obstacle );

    delete &posObstacle_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerPlateForme::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_CreerPlateForme::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_creer_plate_forme );
    ASN1T_Mission_Pion_GEN_CreerPlateForme& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_creer_plate_forme;

    ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


