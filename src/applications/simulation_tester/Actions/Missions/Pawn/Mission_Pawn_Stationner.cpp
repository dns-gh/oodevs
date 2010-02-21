// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_Stationner.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Stationner constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Stationner::Mission_Pawn_Stationner( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Stationner", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Stationner destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Stationner::~Mission_Pawn_Stationner()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Stationner::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Stationner::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_Stationner& asnMission = *new Mission_Pion_Stationner();

    asnMsg_.GetAsnMsg().mission().mission_pion_stationner = &asnMission;

    const Position& pointAttente_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointAttente_, asnMission.point_attente );

    delete &pointAttente_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Stationner::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Stationner::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_stationner ());
    Mission_Pion_Stationner& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_stationner;

    ASN_Tools::Delete( asnMission.point_attente );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


