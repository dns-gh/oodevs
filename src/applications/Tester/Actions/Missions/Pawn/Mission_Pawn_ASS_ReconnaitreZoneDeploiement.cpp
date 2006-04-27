// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ASS_ReconnaitreZoneDeploiement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_ReconnaitreZoneDeploiement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_ReconnaitreZoneDeploiement::Mission_Pawn_ASS_ReconnaitreZoneDeploiement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASS_ReconnaitreZoneDeploiement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_ReconnaitreZoneDeploiement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_ReconnaitreZoneDeploiement::~Mission_Pawn_ASS_ReconnaitreZoneDeploiement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_ReconnaitreZoneDeploiement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_ReconnaitreZoneDeploiement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement& asnMission = *new ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_reconnaitre_zone_deploiement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_reconnaitre_zone_deploiement = &asnMission;

    const Position& positionDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionDeploiement_, asnMission.position_deploiement );

    delete &positionDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_ReconnaitreZoneDeploiement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_ReconnaitreZoneDeploiement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_ass_reconnaitre_zone_deploiement );
    ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_reconnaitre_zone_deploiement;

    ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


