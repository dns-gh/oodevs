// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_NBC_ReconnaitreItineraire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreItineraire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_NBC_ReconnaitreItineraire::Mission_Pawn_NBC_ReconnaitreItineraire( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_NBC_ReconnaitreItineraire", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreItineraire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_NBC_ReconnaitreItineraire::~Mission_Pawn_NBC_ReconnaitreItineraire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreItineraire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_NBC_ReconnaitreItineraire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_NBC_ReconnaitreItineraire& asnMission = *new Mission_Pion_NBC_ReconnaitreItineraire();

    asnMsg_.GetAsnMsg().mission().mission_pion_nbc_reconnaitre_itineraire = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );

    delete &itineraire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreItineraire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_NBC_ReconnaitreItineraire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_nbc_reconnaitre_itineraire ());
    Mission_Pion_NBC_ReconnaitreItineraire& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_nbc_reconnaitre_itineraire;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


