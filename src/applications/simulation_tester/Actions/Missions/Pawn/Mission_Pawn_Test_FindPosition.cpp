// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_Test_FindPosition.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_FindPosition constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_FindPosition::Mission_Pawn_Test_FindPosition( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Test_FindPosition", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_FindPosition destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_FindPosition::~Mission_Pawn_Test_FindPosition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_FindPosition::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_FindPosition::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_FindPosition& asnMission = *new ASN1T_Mission_Pion_Test_FindPosition();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_find_position;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_find_position = &asnMission;

    const Position& point_ = pTarget_->GetTestParam_Point();
    const Position& retreat_ = pTarget_->GetTestParam_Point();
    const T_IdVector& unite_ = pTarget_->GetTestParam_AgentList();

    ASN_Tools::CopyPoint( point_, asnMission.point );
    ASN_Tools::CopyPoint( retreat_, asnMission.retreat );
    ASN_Tools::CopyAgentList( unite_, asnMission.unite );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 4 ), asnMission.type_objectif );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 9 ), asnMission.type_operation );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.avoid );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.reco );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.no_obstacle_avoid );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.debordement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.dec_points );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.niveau_identification );
    ASN_Tools::CopyDirection( pTarget_->GetTestParam_Direction(), asnMission.dgo );

    delete &point_;
    delete &retreat_;
    delete &unite_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_FindPosition::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_FindPosition::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_test_find_position );
    ASN1T_Mission_Pion_Test_FindPosition& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_test_find_position;

    ASN_Tools::Delete( asnMission.point );
    ASN_Tools::Delete( asnMission.retreat );
    ASN_Tools::Delete( asnMission.unite );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


