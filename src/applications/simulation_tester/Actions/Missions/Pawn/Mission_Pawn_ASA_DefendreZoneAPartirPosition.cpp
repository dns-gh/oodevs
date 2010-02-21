// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ASA_DefendreZoneAPartirPosition.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreZoneAPartirPosition constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_DefendreZoneAPartirPosition::Mission_Pawn_ASA_DefendreZoneAPartirPosition( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASA_DefendreZoneAPartirPosition", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreZoneAPartirPosition destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_DefendreZoneAPartirPosition::~Mission_Pawn_ASA_DefendreZoneAPartirPosition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreZoneAPartirPosition::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_DefendreZoneAPartirPosition::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ASA_DefendreZoneAPartirPosition& asnMission = *new Mission_Pion_ASA_DefendreZoneAPartirPosition();

    asnMsg_.GetAsnMsg().mission().mission_pion_asa_defendre_zone_a_partir_position = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();
    const Position& pointDeDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );
    ASN_Tools::CopyPoint( pointDeDeploiement_, asnMission.point_de_deploiement );

    delete &zone_;
    delete &pointDeDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreZoneAPartirPosition::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_DefendreZoneAPartirPosition::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_asa_defendre_zone_a_partir_position ());
    Mission_Pion_ASA_DefendreZoneAPartirPosition& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_asa_defendre_zone_a_partir_position;

    ASN_Tools::Delete( asnMission.zone );
    ASN_Tools::Delete( asnMission.point_de_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


