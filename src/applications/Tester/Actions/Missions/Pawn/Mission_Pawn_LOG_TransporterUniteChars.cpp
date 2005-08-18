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
#include "Mission_Pawn_LOG_TransporterUniteChars.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TransporterUniteChars constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_TransporterUniteChars::Mission_Pawn_LOG_TransporterUniteChars( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_LOG_TransporterUniteChars";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TransporterUniteChars destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_TransporterUniteChars::~Mission_Pawn_LOG_TransporterUniteChars()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TransporterUniteChars::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_TransporterUniteChars::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_TransporterUniteChars& asnMission = *new ASN1T_Mission_Pion_LOG_TransporterUniteChars();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_transporter_unite_chars;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_transporter_unite_chars = &asnMission;

    const T_IdVector& unitesATransporter_ = pTarget_->GetTestParam_AgentList();
    const Position& pointRendezVous_ = pTarget_->GetTestParam_Point();
    const Position& pointDestination_ = pTarget_->GetTestParam_Point();
    const Path& itineraire_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyAgentList( unitesATransporter_, asnMission.unites_a_transporter );
    ASN_Tools::CopyPoint( pointRendezVous_, asnMission.point_rendez_vous );
    ASN_Tools::CopyPoint( pointDestination_, asnMission.point_destination );
    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );

    delete &unitesATransporter_;
    delete &pointRendezVous_;
    delete &pointDestination_;
    delete &itineraire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TransporterUniteChars::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_TransporterUniteChars::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_transporter_unite_chars );
    ASN1T_Mission_Pion_LOG_TransporterUniteChars& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_transporter_unite_chars;

    ASN_Tools::Delete( asnMission.unites_a_transporter );
    ASN_Tools::Delete( asnMission.point_rendez_vous );
    ASN_Tools::Delete( asnMission.point_destination );
    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


