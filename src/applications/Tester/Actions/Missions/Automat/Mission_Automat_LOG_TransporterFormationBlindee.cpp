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
#include "Mission_Automat_LOG_TransporterFormationBlindee.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_TransporterFormationBlindee constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_TransporterFormationBlindee::Mission_Automat_LOG_TransporterFormationBlindee( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_LOG_TransporterFormationBlindee";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_TransporterFormationBlindee destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_TransporterFormationBlindee::~Mission_Automat_LOG_TransporterFormationBlindee()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_TransporterFormationBlindee::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_TransporterFormationBlindee::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_LOG_TransporterFormationBlindee& asnMission = *new ASN1T_Mission_Automate_LOG_TransporterFormationBlindee();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_transporter_formation_blindee;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_log_transporter_formation_blindee = &asnMission;

    const T_IdVector& unitesATransporter_ = pTarget_->GetTestParam_AgentList();
    const Position& pointRendezVous_ = pTarget_->GetTestParam_Point();
    const Position& pointDestination_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyAgentList( unitesATransporter_, asnMission.unites_a_transporter );
    ASN_Tools::CopyPoint( pointRendezVous_, asnMission.point_rendez_vous );
    ASN_Tools::CopyPoint( pointDestination_, asnMission.point_destination );

    delete &unitesATransporter_;
    delete &pointRendezVous_;
    delete &pointDestination_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_TransporterFormationBlindee::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_TransporterFormationBlindee::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_log_transporter_formation_blindee );
    ASN1T_Mission_Automate_LOG_TransporterFormationBlindee& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_log_transporter_formation_blindee;

    ASN_Tools::Delete( asnMission.unites_a_transporter );
    ASN_Tools::Delete( asnMission.point_rendez_vous );
    ASN_Tools::Delete( asnMission.point_destination );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




