// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_ReconnaitreItineraire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReconnaitreItineraire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ReconnaitreItineraire::Mission_Pawn_GEN_ReconnaitreItineraire( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_ReconnaitreItineraire", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReconnaitreItineraire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ReconnaitreItineraire::~Mission_Pawn_GEN_ReconnaitreItineraire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReconnaitreItineraire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ReconnaitreItineraire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_GEN_ReconnaitreItineraire& asnMission = *new Mission_Pion_GEN_ReconnaitreItineraire();

    asnMsg_.GetAsnMsg().mission().mission_pion_gen_reconnaitre_itineraire = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );

    delete &itineraire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReconnaitreItineraire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ReconnaitreItineraire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_gen_reconnaitre_itineraire ());
    Mission_Pion_GEN_ReconnaitreItineraire& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_gen_reconnaitre_itineraire;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


