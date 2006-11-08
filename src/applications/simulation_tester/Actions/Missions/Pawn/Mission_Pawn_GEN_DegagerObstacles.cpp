// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_DegagerObstacles.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerObstacles constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DegagerObstacles::Mission_Pawn_GEN_DegagerObstacles( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_DegagerObstacles", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerObstacles destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DegagerObstacles::~Mission_Pawn_GEN_DegagerObstacles()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerObstacles::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DegagerObstacles::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_DegagerObstacles& asnMission = *new ASN1T_Mission_Pion_GEN_DegagerObstacles();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_degager_obstacles;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_degager_obstacles = &asnMission;

    const T_IdVector& obstacles_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyObjectKnowledgeList( obstacles_, asnMission.obstacles );

    delete &obstacles_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerObstacles::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DegagerObstacles::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_degager_obstacles );
    ASN1T_Mission_Pion_GEN_DegagerObstacles& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_degager_obstacles;

    ASN_Tools::Delete( asnMission.obstacles );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


