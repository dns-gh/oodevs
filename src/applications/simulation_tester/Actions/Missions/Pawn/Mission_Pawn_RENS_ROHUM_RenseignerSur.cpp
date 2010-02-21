// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_RENS_ROHUM_RenseignerSur.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_RenseignerSur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROHUM_RenseignerSur::Mission_Pawn_RENS_ROHUM_RenseignerSur( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_RENS_ROHUM_RenseignerSur", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_RenseignerSur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROHUM_RenseignerSur::~Mission_Pawn_RENS_ROHUM_RenseignerSur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_RenseignerSur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROHUM_RenseignerSur::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_RENS_ROHUM_RenseignerSur& asnMission = *new Mission_Pion_RENS_ROHUM_RenseignerSur();

    asnMsg_.GetAsnMsg().mission().mission_pion_rens_rohum_renseigner_sur = &asnMission;

    const Location& zoneAObserver_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneAObserver_, asnMission.zone_a_observer );

    delete &zoneAObserver_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_RenseignerSur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROHUM_RenseignerSur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_rens_rohum_renseigner_sur ());
    Mission_Pion_RENS_ROHUM_RenseignerSur& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_rens_rohum_renseigner_sur;

    ASN_Tools::Delete( asnMission.zone_a_observer );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


