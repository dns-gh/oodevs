// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_RENS_ROHUM_OrienterGuider.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_OrienterGuider constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROHUM_OrienterGuider::Mission_Pawn_RENS_ROHUM_OrienterGuider( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_RENS_ROHUM_OrienterGuider", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_OrienterGuider destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROHUM_OrienterGuider::~Mission_Pawn_RENS_ROHUM_OrienterGuider()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_OrienterGuider::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROHUM_OrienterGuider::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_RENS_ROHUM_OrienterGuider& asnMission = *new Mission_Pion_RENS_ROHUM_OrienterGuider();

    asnMsg_.GetAsnMsg().mission().mission_pion_rens_rohum_orienter_guider = &asnMission;


    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.ami );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_OrienterGuider::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROHUM_OrienterGuider::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_rens_rohum_orienter_guider ());
    Mission_Pion_RENS_ROHUM_OrienterGuider& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_rens_rohum_orienter_guider;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

