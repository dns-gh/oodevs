// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_ArmerPIA.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ArmerPIA constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_ArmerPIA::Mission_Pawn_LOG_ArmerPIA( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_ArmerPIA", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ArmerPIA destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_ArmerPIA::~Mission_Pawn_LOG_ArmerPIA()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ArmerPIA::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_ArmerPIA::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_LOG_ArmerPIA& asnMission = *new Mission_Pion_LOG_ArmerPIA();

    asnMsg_.GetAsnMsg().mission().mission_pion_log_armer_pia = &asnMission;

    const Position& point_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( point_, asnMission.point );

    delete &point_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ArmerPIA::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_ArmerPIA::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_log_armer_pia ());
    Mission_Pion_LOG_ArmerPIA& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_log_armer_pia;

    ASN_Tools::Delete( asnMission.point );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


