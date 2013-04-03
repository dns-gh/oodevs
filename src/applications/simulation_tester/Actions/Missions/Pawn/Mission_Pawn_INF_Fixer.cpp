// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_INF_Fixer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Fixer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Fixer::Mission_Pawn_INF_Fixer( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_Fixer", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Fixer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Fixer::~Mission_Pawn_INF_Fixer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Fixer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Fixer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_INF_Fixer& asnMission = *new Mission_Pion_INF_Fixer();

    asnMsg_.GetAsnMsg().mission().mission_pion_inf_fixer = &asnMission;


    ASN_Tools::CopyAgentKnowledge( pTarget_->GetTestParam_AgentKnowledge(), asnMission.ennemi );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Fixer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Fixer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_inf_fixer ());
    Mission_Pion_INF_Fixer& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_inf_fixer;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

