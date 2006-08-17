// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_SecuriserZoneContrePopulations.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SecuriserZoneContrePopulations constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_SecuriserZoneContrePopulations::Mission_Pawn_SecuriserZoneContrePopulations( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_SecuriserZoneContrePopulations", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SecuriserZoneContrePopulations destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_SecuriserZoneContrePopulations::~Mission_Pawn_SecuriserZoneContrePopulations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SecuriserZoneContrePopulations::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_SecuriserZoneContrePopulations::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_SecuriserZoneContrePopulations& asnMission = *new ASN1T_Mission_Pion_SecuriserZoneContrePopulations();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_securiser_zone_contre_populations;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_securiser_zone_contre_populations = &asnMission;

    const Position& point_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( point_, asnMission.point );

    delete &point_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SecuriserZoneContrePopulations::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_SecuriserZoneContrePopulations::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_securiser_zone_contre_populations );
    ASN1T_Mission_Pion_SecuriserZoneContrePopulations& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_securiser_zone_contre_populations;

    ASN_Tools::Delete( asnMission.point );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


