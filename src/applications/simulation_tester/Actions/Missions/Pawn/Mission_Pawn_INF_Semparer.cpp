// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_INF_Semparer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Semparer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Semparer::Mission_Pawn_INF_Semparer( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_Semparer", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Semparer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Semparer::~Mission_Pawn_INF_Semparer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Semparer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Semparer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_INF_Semparer& asnMission = *new Mission_Pion_INF_Semparer();

    asnMsg_.GetAsnMsg().mission().mission_pion_inf_semparer = &asnMission;

    const Position& objectif_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( objectif_, asnMission.objectif );

    delete &objectif_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Semparer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Semparer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_inf_semparer ());
    Mission_Pion_INF_Semparer& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_inf_semparer;

    ASN_Tools::Delete( asnMission.objectif );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


