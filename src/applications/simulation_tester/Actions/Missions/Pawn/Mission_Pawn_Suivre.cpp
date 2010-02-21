// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_Suivre.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Suivre constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Suivre::Mission_Pawn_Suivre( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Suivre", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Suivre destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Suivre::~Mission_Pawn_Suivre()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Suivre::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Suivre::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_Suivre& asnMission = *new Mission_Pion_Suivre();

    asnMsg_.GetAsnMsg().mission().mission_pion_suivre = &asnMission;


    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.unite_a_suivre );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Suivre::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Suivre::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_suivre ());
    Mission_Pion_Suivre& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_suivre;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


