// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_TransporterUnite.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TransporterUnite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_TransporterUnite::Mission_Pawn_LOG_TransporterUnite( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_TransporterUnite", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TransporterUnite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_TransporterUnite::~Mission_Pawn_LOG_TransporterUnite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TransporterUnite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_TransporterUnite::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_TransporterUnite& asnMission = *new ASN1T_Mission_Pion_LOG_TransporterUnite();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_$$$$NotDefined$$$$;
    asnMsg_.GetAsnMsg().mission.u.$$$$NotDefined$$$$ = &asnMission;

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
// Name: Mission_Pawn_LOG_TransporterUnite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_TransporterUnite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_$$$$NotDefined$$$$ );
    ASN1T_Mission_Pion_LOG_TransporterUnite& asnMission = *asnMsg_.GetAsnMsg().mission.u.$$$$NotDefined$$$$;

    ASN_Tools::Delete( asnMission.unites_a_transporter );
    ASN_Tools::Delete( asnMission.point_rendez_vous );
    ASN_Tools::Delete( asnMission.point_destination );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


