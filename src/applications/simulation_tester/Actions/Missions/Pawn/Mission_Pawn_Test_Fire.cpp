// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_Test_Fire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Fire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_Fire::Mission_Pawn_Test_Fire( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Test_Fire", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Fire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_Fire::~Mission_Pawn_Test_Fire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Fire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_Fire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_Fire& asnMission = *new ASN1T_Mission_Pion_Test_Fire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_fire;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_fire = &asnMission;

    const Position& point_ = pTarget_->GetTestParam_Point();
    const T_IdVector& targets_ = pTarget_->GetTestParam_AgentKnowledgeList();

    ASN_Tools::CopyPoint( point_, asnMission.point );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.tir_indirect );
    ASN_Tools::CopyDotationDType( pTarget_->GetTestParam_DotationDType(), asnMission.munitions );
    ASN_Tools::CopyNumeric( pTarget_->GetTestParam_Numeric( 0, 4294967295 ), asnMission.nb_it );
    ASN_Tools::CopyAgentKnowledgeList( targets_, asnMission.targets );

    delete &point_;
    delete &targets_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Fire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_Fire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_test_fire );
    ASN1T_Mission_Pion_Test_Fire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_test_fire;

    ASN_Tools::Delete( asnMission.point );
    ASN_Tools::Delete( asnMission.targets );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


