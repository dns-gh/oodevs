// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

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
    : Mission_Pawn_ABC ( "Pawn_JOINT_MARINE_Transporter", pawn )
{
    // NOTHING
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

    const T_IdVector& unitesATransporter_ = pTarget_->GetTestParam_AgentList();
    const Position& pointDebarquement_ = pTarget_->GetTestParam_Point();
    const Position& pointEmbarquement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyAgentList( unitesATransporter_, asnMission.unites_a_transporter );
    ASN_Tools::CopyPoint( pointDebarquement_, asnMission.point_debarquement );
    ASN_Tools::CopyPoint( pointEmbarquement_, asnMission.point_embarquement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.avec_materiel );

    delete &unitesATransporter_;
    delete &pointDebarquement_;
    delete &pointEmbarquement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_JOINT_MARINE_Transporter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_JOINT_MARINE_Transporter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_joint_marine_transporter );
    ASN1T_Mission_Pion_JOINT_MARINE_Transporter& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_joint_marine_transporter;

    ASN_Tools::Delete( asnMission.unites_a_transporter );
    ASN_Tools::Delete( asnMission.point_debarquement );
    ASN_Tools::Delete( asnMission.point_embarquement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


