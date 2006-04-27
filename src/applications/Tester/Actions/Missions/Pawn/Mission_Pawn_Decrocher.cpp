// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_Decrocher.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Decrocher constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Decrocher::Mission_Pawn_Decrocher( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Decrocher", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Decrocher destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Decrocher::~Mission_Pawn_Decrocher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Decrocher::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Decrocher::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Decrocher& asnMission = *new ASN1T_Mission_Pion_Decrocher();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_decrocher;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_decrocher = &asnMission;

    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );

    delete &pointRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Decrocher::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Decrocher::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_decrocher );
    ASN1T_Mission_Pion_Decrocher& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_decrocher;

    ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


