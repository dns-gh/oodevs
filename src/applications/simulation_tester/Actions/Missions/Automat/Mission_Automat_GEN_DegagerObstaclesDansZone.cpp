// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_GEN_DegagerObstaclesDansZone.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_DegagerObstaclesDansZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_DegagerObstaclesDansZone::Mission_Automat_GEN_DegagerObstaclesDansZone( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_DegagerObstaclesDansZone", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_DegagerObstaclesDansZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_DegagerObstaclesDansZone::~Mission_Automat_GEN_DegagerObstaclesDansZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_DegagerObstaclesDansZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_DegagerObstaclesDansZone::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_DegagerObstaclesDansZone& asnMission = *new ASN1T_Mission_Automate_GEN_DegagerObstaclesDansZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_degager_obstacles_dans_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_degager_obstacles_dans_zone = &asnMission;

    const T_IdVector& obstacles_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyObjectKnowledgeList( obstacles_, asnMission.obstacles );

    delete &obstacles_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_DegagerObstaclesDansZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_DegagerObstaclesDansZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_degager_obstacles_dans_zone );
    ASN1T_Mission_Automate_GEN_DegagerObstaclesDansZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_degager_obstacles_dans_zone;

    ASN_Tools::Delete( asnMission.obstacles );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




