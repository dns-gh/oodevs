// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_FaireMouvement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_FaireMouvement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_FaireMouvement::Mission_Pawn_FaireMouvement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_FaireMouvement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_FaireMouvement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_FaireMouvement::~Mission_Pawn_FaireMouvement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_FaireMouvement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_FaireMouvement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_FaireMouvement& asnMission = *new Mission_Pion_FaireMouvement();

    asnMsg_.GetAsnMsg().mission().mission_pion_faire_mouvement = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );

    delete &itineraire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_FaireMouvement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_FaireMouvement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_faire_mouvement ());
    Mission_Pion_FaireMouvement& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_faire_mouvement;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


