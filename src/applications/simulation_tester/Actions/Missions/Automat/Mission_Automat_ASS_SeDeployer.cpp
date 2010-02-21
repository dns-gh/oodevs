// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ASS_SeDeployer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_SeDeployer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASS_SeDeployer::Mission_Automat_ASS_SeDeployer( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ASS_SeDeployer", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_SeDeployer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASS_SeDeployer::~Mission_Automat_ASS_SeDeployer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_SeDeployer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ASS_SeDeployer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ASS_SeDeployer& asnMission = *new Mission_Automate_ASS_SeDeployer();

    asnMsg_.GetAsnMsg().mission().mission_automate_ass_se_deployer = &asnMission;

    const Location& zoneDeploiement_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneDeploiement_, asnMission.zone_deploiement );

    delete &zoneDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_SeDeployer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASS_SeDeployer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_ass_se_deployer ());
    Mission_Automate_ASS_SeDeployer& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_ass_se_deployer;

    ASN_Tools::Delete( asnMission.zone_deploiement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




