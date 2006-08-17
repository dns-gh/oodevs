// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_Test_MoveTo.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_MoveTo constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_MoveTo::Mission_Pawn_Test_MoveTo( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Test_MoveTo", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_MoveTo destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_MoveTo::~Mission_Pawn_Test_MoveTo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_MoveTo::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_MoveTo::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_MoveTo& asnMission = *new ASN1T_Mission_Pion_Test_MoveTo();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_move_to;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_move_to = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();
    const Position& visionPoint_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 7 ), asnMission.type_itineraire );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.debarque );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.verrouillage_vision );
    ASN_Tools::CopyPoint( visionPoint_, asnMission.vision_point );
    ASN_Tools::CopyDirection( pTarget_->GetTestParam_Direction(), asnMission.vision_direction );

    delete &itineraire_;
    delete &visionPoint_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_MoveTo::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_MoveTo::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_test_move_to );
    ASN1T_Mission_Pion_Test_MoveTo& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_test_move_to;

    ASN_Tools::Delete( asnMission.itineraire );
    ASN_Tools::Delete( asnMission.vision_point );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


