// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_INF_RepondreAAggression.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_RepondreAAggression constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_RepondreAAggression::Mission_Pawn_INF_RepondreAAggression( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_RepondreAAggression", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_RepondreAAggression destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_RepondreAAggression::~Mission_Pawn_INF_RepondreAAggression()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_RepondreAAggression::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_RepondreAAggression::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_INF_RepondreAAggression& asnMission = *new Mission_Pion_INF_RepondreAAggression();

    asnMsg_.GetAsnMsg().mission().mission_pion_inf_repondre_a_aggression = &asnMission;


    ASN_Tools::CopyAgentKnowledge( pTarget_->GetTestParam_AgentKnowledge(), asnMission.aggresseur );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_RepondreAAggression::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_RepondreAAggression::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_inf_repondre_a_aggression ());
    Mission_Pion_INF_RepondreAAggression& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_inf_repondre_a_aggression;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

