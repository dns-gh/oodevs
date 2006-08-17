// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_LOG_TransporterUnites.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_TransporterUnites constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_TransporterUnites::Mission_Automat_LOG_TransporterUnites( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_LOG_TransporterUnites", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_TransporterUnites destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_TransporterUnites::~Mission_Automat_LOG_TransporterUnites()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_TransporterUnites::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_TransporterUnites::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_LOG_TransporterUnites& asnMission = *new ASN1T_Mission_Automate_LOG_TransporterUnites();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_transporter_unites;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_log_transporter_unites = &asnMission;

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
// Name: Mission_Automat_LOG_TransporterUnites::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_TransporterUnites::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_log_transporter_unites );
    ASN1T_Mission_Automate_LOG_TransporterUnites& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_log_transporter_unites;

    ASN_Tools::Delete( asnMission.unites_a_transporter );
    ASN_Tools::Delete( asnMission.point_rendez_vous );
    ASN_Tools::Delete( asnMission.point_destination );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




