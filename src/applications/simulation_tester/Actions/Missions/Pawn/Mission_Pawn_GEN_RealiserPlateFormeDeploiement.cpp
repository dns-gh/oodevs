// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_RealiserPlateFormeDeploiement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserPlateFormeDeploiement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserPlateFormeDeploiement::Mission_Pawn_GEN_RealiserPlateFormeDeploiement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_RealiserPlateFormeDeploiement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserPlateFormeDeploiement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserPlateFormeDeploiement::~Mission_Pawn_GEN_RealiserPlateFormeDeploiement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserPlateFormeDeploiement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserPlateFormeDeploiement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_plate_forme_deploiement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_plate_forme_deploiement = &asnMission;

    const Position& positionObstacle_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionObstacle_, asnMission.position_obstacle );

    delete &positionObstacle_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserPlateFormeDeploiement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserPlateFormeDeploiement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_plate_forme_deploiement );
    ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_plate_forme_deploiement;

    ASN_Tools::Delete( asnMission.position_obstacle );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


