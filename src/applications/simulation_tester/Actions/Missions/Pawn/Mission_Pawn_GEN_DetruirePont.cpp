// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_GEN_DetruirePont.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DetruirePont constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DetruirePont::Mission_Pawn_GEN_DetruirePont( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_DetruirePont", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DetruirePont destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DetruirePont::~Mission_Pawn_GEN_DetruirePont()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DetruirePont::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DetruirePont::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_DetruirePont& asnMission = *new ASN1T_Mission_Pion_GEN_DetruirePont();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_detruire_pont;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_detruire_pont = &asnMission;

    const Position& posObstacle_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( posObstacle_, asnMission.pos_obstacle );
    ASN_Tools::CopyID( pTarget_->GetTestParam_ID(), asnMission.oid_obstacle_planifie );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 1 ), asnMission.preliminaire );

    delete &posObstacle_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DetruirePont::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DetruirePont::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_detruire_pont );
    ASN1T_Mission_Pion_GEN_DetruirePont& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_detruire_pont;

    ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


