// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_LOG_ReconnaitreZoneDeDeploiement.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ReconnaitreZoneDeDeploiement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_ReconnaitreZoneDeDeploiement::Mission_Automat_LOG_ReconnaitreZoneDeDeploiement( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_LOG_ReconnaitreZoneDeDeploiement", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ReconnaitreZoneDeDeploiement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_ReconnaitreZoneDeDeploiement::~Mission_Automat_LOG_ReconnaitreZoneDeDeploiement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ReconnaitreZoneDeDeploiement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_ReconnaitreZoneDeDeploiement::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_LOG_ReconnaitreZoneDeDeploiement& asnMission = *new Mission_Automate_LOG_ReconnaitreZoneDeDeploiement();

    asnMsg_.GetAsnMsg().mission().mission_automate_log_reconnaitre_zone_de_deploiement = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ReconnaitreZoneDeDeploiement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_ReconnaitreZoneDeDeploiement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_log_reconnaitre_zone_de_deploiement ());
    Mission_Automate_LOG_ReconnaitreZoneDeDeploiement& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_log_reconnaitre_zone_de_deploiement;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




