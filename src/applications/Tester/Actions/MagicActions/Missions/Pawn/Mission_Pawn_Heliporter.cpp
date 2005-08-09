// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
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
#include "Mission_Pawn_Heliporter.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Heliporter constructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
Mission_Pawn_Heliporter::Mission_Pawn_Heliporter( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Heliporter destructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
Mission_Pawn_Heliporter::~Mission_Pawn_Heliporter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Heliporter::Serialize
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Heliporter::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_Heliporter& asnMission             = *new ASN1T_Mission_Pion_Test_Heliporter();
    asnMsg_.GetAsnMsg().mission.t                              = T_Mission_Pion_mission_pion_test_heliporter;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_heliporter = &asnMission;

    ASN_Tools::CopyAgentList( pTarget_->GetTP_PawnList(), asnMission.agents             );
    ASN_Tools::CopyPosition ( pTarget_->GetTP_Position(), asnMission.point_embarquement );
    ASN_Tools::CopyPosition ( pTarget_->GetTP_Position(), asnMission.point_debarquement );
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Heliporter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Heliporter::Clean()
{
    //delete &asnMsg_.GetAsnMsg().mission.u.mission_pion_test_heliporter;
    Mission_Pawn_ABC::Clean();
}
