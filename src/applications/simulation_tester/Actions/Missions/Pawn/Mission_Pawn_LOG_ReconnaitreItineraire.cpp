// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_ReconnaitreItineraire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ReconnaitreItineraire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_ReconnaitreItineraire::Mission_Pawn_LOG_ReconnaitreItineraire( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_ReconnaitreItineraire", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ReconnaitreItineraire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_ReconnaitreItineraire::~Mission_Pawn_LOG_ReconnaitreItineraire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ReconnaitreItineraire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_ReconnaitreItineraire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_ReconnaitreItineraire& asnMission = *new ASN1T_Mission_Pion_LOG_ReconnaitreItineraire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_reconnaitre_itineraire;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_reconnaitre_itineraire = &asnMission;

    const Path& itineraireLog_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraireLog_, asnMission.itineraire_log );

    delete &itineraireLog_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ReconnaitreItineraire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_ReconnaitreItineraire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_reconnaitre_itineraire );
    ASN1T_Mission_Pion_LOG_ReconnaitreItineraire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_reconnaitre_itineraire;

    ASN_Tools::Delete( asnMission.itineraire_log );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


