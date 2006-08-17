// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement::Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_ReconnaitreZoneDeDeploiement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement::~Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement& asnMission = *new ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_reconnaitre_zone_de_deploiement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_reconnaitre_zone_de_deploiement = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_reconnaitre_zone_de_deploiement );
    ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_reconnaitre_zone_de_deploiement;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


