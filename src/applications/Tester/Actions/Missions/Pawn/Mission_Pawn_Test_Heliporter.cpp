// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
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
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_Test_Heliporter";
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

    ASN1T_Mission_Pion_Test_Heliporter& asnMission = *new ASN1T_Mission_Pion_Test_Heliporter();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_heliporter;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_heliporter = &asnMission;

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
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_test_heliporter );
    ASN1T_Mission_Pion_Test_Heliporter& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_test_heliporter;

    ASN_Tools::Delete( asnMission.agents );
    ASN_Tools::Delete( asnMission.point_embarquement );
    ASN_Tools::Delete( asnMission.point_debarquement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


