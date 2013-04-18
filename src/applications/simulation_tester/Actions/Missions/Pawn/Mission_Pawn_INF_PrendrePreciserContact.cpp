// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_INF_PrendrePreciserContact.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_PrendrePreciserContact constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_PrendrePreciserContact::Mission_Pawn_INF_PrendrePreciserContact( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_PrendrePreciserContact", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_PrendrePreciserContact destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_PrendrePreciserContact::~Mission_Pawn_INF_PrendrePreciserContact()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_PrendrePreciserContact::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_PrendrePreciserContact::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_INF_PrendrePreciserContact& asnMission = *new Mission_Pion_INF_PrendrePreciserContact();

    asnMsg_.GetAsnMsg().mission().mission_pion_inf_prendre_preciser_contact = &asnMission;


    ASN_Tools::CopyAgentKnowledge( pTarget_->GetTestParam_AgentKnowledge(), asnMission.ennemi );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_PrendrePreciserContact::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_PrendrePreciserContact::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_inf_prendre_preciser_contact ());
    Mission_Pion_INF_PrendrePreciserContact& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_inf_prendre_preciser_contact;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

