// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_Transporter.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Transporter constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Transporter::Mission_Automat_Transporter( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_Transporter", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Transporter destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Transporter::~Mission_Automat_Transporter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Transporter::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_Transporter::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_Transporter& asnMission = *new ASN1T_Mission_Automate_Transporter();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_transporter;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_transporter = &asnMission;

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
// Name: Mission_Automat_Transporter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_Transporter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_transporter );
    ASN1T_Mission_Automate_Transporter& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_transporter;

    ASN_Tools::Delete( asnMission.unites_a_transporter );
    ASN_Tools::Delete( asnMission.point_debarquement );
    ASN_Tools::Delete( asnMission.point_embarquement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




