// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_OuvrirItineraire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_OuvrirItineraire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_OuvrirItineraire::Mission_Pawn_GEN_OuvrirItineraire( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_OuvrirItineraire", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_OuvrirItineraire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_OuvrirItineraire::~Mission_Pawn_GEN_OuvrirItineraire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_OuvrirItineraire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_OuvrirItineraire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_OuvrirItineraire& asnMission = *new ASN1T_Mission_Pion_GEN_OuvrirItineraire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_ouvrir_itineraire;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_ouvrir_itineraire = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );

    delete &itineraire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_OuvrirItineraire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_OuvrirItineraire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_ouvrir_itineraire );
    ASN1T_Mission_Pion_GEN_OuvrirItineraire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_ouvrir_itineraire;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


