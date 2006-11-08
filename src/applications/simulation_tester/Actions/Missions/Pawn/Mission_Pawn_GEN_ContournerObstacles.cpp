// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_ContournerObstacles.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ContournerObstacles constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ContournerObstacles::Mission_Pawn_GEN_ContournerObstacles( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_ContournerObstacles", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ContournerObstacles destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ContournerObstacles::~Mission_Pawn_GEN_ContournerObstacles()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ContournerObstacles::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ContournerObstacles::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_ContournerObstacles& asnMission = *new ASN1T_Mission_Pion_GEN_ContournerObstacles();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_contourner_obstacles;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_contourner_obstacles = &asnMission;

    const T_IdVector& obstacles_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyObjectKnowledgeList( obstacles_, asnMission.obstacles );

    delete &obstacles_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ContournerObstacles::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ContournerObstacles::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_contourner_obstacles );
    ASN1T_Mission_Pion_GEN_ContournerObstacles& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_contourner_obstacles;

    ASN_Tools::Delete( asnMission.obstacles );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


