// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_Test_Heliporter.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Heliporter constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_Heliporter::Mission_Pawn_Test_Heliporter( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Test_Heliporter", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Heliporter destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_Heliporter::~Mission_Pawn_Test_Heliporter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Heliporter::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_Heliporter::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_Test_Heliporter& asnMission = *new Mission_Pion_Test_Heliporter();

    asnMsg_.GetAsnMsg().mission().mission_pion_test_heliporter = &asnMission;

    const T_IdVector& agents_ = pTarget_->GetTestParam_AgentList();
    const Position& pointEmbarquement_ = pTarget_->GetTestParam_Point();
    const Position& pointDebarquement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyAgentList( agents_, asnMission.agents );
    ASN_Tools::CopyPoint( pointEmbarquement_, asnMission.point_embarquement );
    ASN_Tools::CopyPoint( pointDebarquement_, asnMission.point_debarquement );

    delete &agents_;
    delete &pointEmbarquement_;
    delete &pointDebarquement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Heliporter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_Heliporter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_test_heliporter ());
    Mission_Pion_Test_Heliporter& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_test_heliporter;

    ASN_Tools::Delete( asnMission.agents );
    ASN_Tools::Delete( asnMission.point_embarquement );
    ASN_Tools::Delete( asnMission.point_debarquement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

