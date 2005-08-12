// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
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
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_INF_RepondreAAggression";
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

    ASN1T_Mission_Pion_INF_RepondreAAggression& asnMission = *new ASN1T_Mission_Pion_INF_RepondreAAggression();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_repondre_a_aggression;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_repondre_a_aggression = &asnMission;

    ASN_Tools::CopyAgentKnowledge( pTarget_->GetTestParam_AgentKnowledge(), asnMission.aggresseur );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_RepondreAAggression::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_RepondreAAggression::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_repondre_a_aggression );
    ASN1T_Mission_Pion_INF_RepondreAAggression& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_repondre_a_aggression;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


