// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_INF_Couvrir.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Couvrir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Couvrir::Mission_Pawn_INF_Couvrir( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_Couvrir", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Couvrir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Couvrir::~Mission_Pawn_INF_Couvrir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Couvrir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Couvrir::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_INF_Couvrir& asnMission = *new Mission_Pion_INF_Couvrir();

    asnMsg_.GetAsnMsg().mission().mission_pion_inf_couvrir = &asnMission;

    const Position& pointCouverture_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointCouverture_, asnMission.point_couverture );
    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.cible );

    delete &pointCouverture_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Couvrir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Couvrir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_inf_couvrir ());
    Mission_Pion_INF_Couvrir& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_inf_couvrir;

    ASN_Tools::Delete( asnMission.point_couverture );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


