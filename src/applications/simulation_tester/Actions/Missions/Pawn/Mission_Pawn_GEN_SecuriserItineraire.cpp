// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_GEN_SecuriserItineraire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_SecuriserItineraire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_SecuriserItineraire::Mission_Pawn_GEN_SecuriserItineraire( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_SecuriserItineraire", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_SecuriserItineraire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_SecuriserItineraire::~Mission_Pawn_GEN_SecuriserItineraire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_SecuriserItineraire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_SecuriserItineraire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_SecuriserItineraire& asnMission = *new ASN1T_Mission_Pion_GEN_SecuriserItineraire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_securiser_itineraire;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_securiser_itineraire = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );

    delete &itineraire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_SecuriserItineraire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_SecuriserItineraire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_securiser_itineraire );
    ASN1T_Mission_Pion_GEN_SecuriserItineraire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_securiser_itineraire;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


