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
#include "Mission_Pawn_JOINT_MARINE_Transporter.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_JOINT_MARINE_Transporter constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_JOINT_MARINE_Transporter::Mission_Pawn_JOINT_MARINE_Transporter( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_JOINT_MARINE_Transporter";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_JOINT_MARINE_Transporter destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_JOINT_MARINE_Transporter::~Mission_Pawn_JOINT_MARINE_Transporter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_JOINT_MARINE_Transporter::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_JOINT_MARINE_Transporter::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_JOINT_MARINE_Transporter& asnMission = *new ASN1T_Mission_Pion_JOINT_MARINE_Transporter();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_joint_marine_transporter;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_joint_marine_transporter = &asnMission;

    ASN_Tools::CopyAgentList( pTarget_->GetTestParam_AgentList(), asnMission.unites_a_transporter );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_debarquement );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_embarquement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.avec_materiel );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_JOINT_MARINE_Transporter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_JOINT_MARINE_Transporter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_joint_marine_transporter );
    ASN1T_Mission_Pion_JOINT_MARINE_Transporter& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_joint_marine_transporter;

    ASN_Tools::Delete( asnMission.point_debarquement );
    ASN_Tools::Delete( asnMission.point_embarquement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


