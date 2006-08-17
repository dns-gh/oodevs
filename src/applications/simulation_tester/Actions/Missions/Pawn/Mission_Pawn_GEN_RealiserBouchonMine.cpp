// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_GEN_RealiserBouchonMine.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserBouchonMine constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserBouchonMine::Mission_Pawn_GEN_RealiserBouchonMine( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_RealiserBouchonMine", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserBouchonMine destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserBouchonMine::~Mission_Pawn_GEN_RealiserBouchonMine()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserBouchonMine::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserBouchonMine::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserBouchonMine& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserBouchonMine();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_bouchon_mine;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_bouchon_mine = &asnMission;

    const Position& posObstacle_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( posObstacle_, asnMission.pos_obstacle );
    ASN_Tools::CopyID( pTarget_->GetTestParam_ID(), asnMission.oid_obstacle_planifie );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 1 ), asnMission.urgence );

    delete &posObstacle_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserBouchonMine::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserBouchonMine::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_bouchon_mine );
    ASN1T_Mission_Pion_GEN_RealiserBouchonMine& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_bouchon_mine;

    ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


