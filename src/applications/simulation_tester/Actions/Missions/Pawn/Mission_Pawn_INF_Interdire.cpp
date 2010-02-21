// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_INF_Interdire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Interdire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Interdire::Mission_Pawn_INF_Interdire( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_Interdire", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Interdire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Interdire::~Mission_Pawn_INF_Interdire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Interdire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Interdire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_INF_Interdire& asnMission = *new Mission_Pion_INF_Interdire();

    asnMsg_.GetAsnMsg().mission().mission_pion_inf_interdire = &asnMission;

    const Position& pointAInterdire_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointAInterdire_, asnMission.point_a_interdire );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.preparer_terrain );

    delete &pointAInterdire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Interdire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Interdire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_inf_interdire ());
    Mission_Pion_INF_Interdire& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_inf_interdire;

    ASN_Tools::Delete( asnMission.point_a_interdire );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


