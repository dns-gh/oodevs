// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_RealiserEboulement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserEboulement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserEboulement::Mission_Pawn_GEN_RealiserEboulement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_RealiserEboulement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserEboulement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserEboulement::~Mission_Pawn_GEN_RealiserEboulement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserEboulement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserEboulement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserEboulement& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserEboulement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_eboulement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_eboulement = &asnMission;

    const Position& posObstacle_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( posObstacle_, asnMission.pos_obstacle );
    ASN_Tools::CopyID( pTarget_->GetTestParam_ID(), asnMission.oid_obstacle_planifie );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 1 ), asnMission.preliminaire );

    delete &posObstacle_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserEboulement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserEboulement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_eboulement );
    ASN1T_Mission_Pion_GEN_RealiserEboulement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_eboulement;

    ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


