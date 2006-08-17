// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ABC_ReconnaitrePoint.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReconnaitrePoint constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ReconnaitrePoint::Mission_Pawn_ABC_ReconnaitrePoint( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_ReconnaitrePoint", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReconnaitrePoint destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ReconnaitrePoint::~Mission_Pawn_ABC_ReconnaitrePoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReconnaitrePoint::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ReconnaitrePoint::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_ReconnaitrePoint& asnMission = *new ASN1T_Mission_Pion_ABC_ReconnaitrePoint();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_reconnaitre_point;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_reconnaitre_point = &asnMission;

    const Position& pointAReconnaitre_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointAReconnaitre_, asnMission.point_a_reconnaitre );

    delete &pointAReconnaitre_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReconnaitrePoint::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ReconnaitrePoint::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_reconnaitre_point );
    ASN1T_Mission_Pion_ABC_ReconnaitrePoint& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_reconnaitre_point;

    ASN_Tools::Delete( asnMission.point_a_reconnaitre );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


