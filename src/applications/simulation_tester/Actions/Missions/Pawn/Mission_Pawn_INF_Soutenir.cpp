// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_INF_Soutenir.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Soutenir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Soutenir::Mission_Pawn_INF_Soutenir( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_Soutenir", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Soutenir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Soutenir::~Mission_Pawn_INF_Soutenir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Soutenir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Soutenir::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_INF_Soutenir& asnMission = *new ASN1T_Mission_Pion_INF_Soutenir();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_soutenir;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_soutenir = &asnMission;


    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.unite_a_soutenir );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Soutenir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Soutenir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_soutenir );
    ASN1T_Mission_Pion_INF_Soutenir& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_soutenir;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


