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
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_LOG_ReconnaitreZoneDeDeploiement";
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

    ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement& asnMission = *new ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_reconnaitre_zone_de_deploiement;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_log_reconnaitre_zone_de_deploiement = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ReconnaitreZoneDeDeploiement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_ReconnaitreZoneDeDeploiement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_log_reconnaitre_zone_de_deploiement );
    ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_log_reconnaitre_zone_de_deploiement;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




