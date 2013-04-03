// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_PlastronEnnemi.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_PlastronEnnemi constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_PlastronEnnemi::Mission_Pawn_PlastronEnnemi( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_PlastronEnnemi", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_PlastronEnnemi destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_PlastronEnnemi::~Mission_Pawn_PlastronEnnemi()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_PlastronEnnemi::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_PlastronEnnemi::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_PlastronEnnemi& asnMission = *new Mission_Pion_PlastronEnnemi();

    asnMsg_.GetAsnMsg().mission().mission_pion_plastron_ennemi = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();
    const Position& pointMission_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );
    ASN_Tools::CopyPoint( pointMission_, asnMission.point_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.type );

    delete &itineraire_;
    delete &pointMission_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_PlastronEnnemi::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_PlastronEnnemi::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_plastron_ennemi ());
    Mission_Pion_PlastronEnnemi& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_plastron_ennemi;

    ASN_Tools::Delete( asnMission.itineraire );
    ASN_Tools::Delete( asnMission.point_mission );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

