// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_GEN_ActiverObstacle.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ActiverObstacle constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ActiverObstacle::Mission_Pawn_GEN_ActiverObstacle( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_ActiverObstacle", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ActiverObstacle destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ActiverObstacle::~Mission_Pawn_GEN_ActiverObstacle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ActiverObstacle::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ActiverObstacle::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_ActiverObstacle& asnMission = *new ASN1T_Mission_Pion_GEN_ActiverObstacle();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_activer_obstacle;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_activer_obstacle = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.obstacle );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ActiverObstacle::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ActiverObstacle::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_activer_obstacle );
    ASN1T_Mission_Pion_GEN_ActiverObstacle& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_activer_obstacle;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


