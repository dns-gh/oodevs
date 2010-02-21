// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
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

    Mission_Pion_GEN_ActiverObstacle& asnMission = *new Mission_Pion_GEN_ActiverObstacle();

    asnMsg_.GetAsnMsg().mission().mission_pion_gen_activer_obstacle = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.obstacle );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ActiverObstacle::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ActiverObstacle::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_gen_activer_obstacle ());
    Mission_Pion_GEN_ActiverObstacle& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_gen_activer_obstacle;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


