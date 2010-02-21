// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_LOG_SeDeployer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_SeDeployer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_SeDeployer::Mission_Automat_LOG_SeDeployer( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_LOG_SeDeployer", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_SeDeployer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_SeDeployer::~Mission_Automat_LOG_SeDeployer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_SeDeployer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_SeDeployer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_LOG_SeDeployer& asnMission = *new Mission_Automate_LOG_SeDeployer();

    asnMsg_.GetAsnMsg().mission().mission_automate_log_se_deployer = &asnMission;

    const Position& positionDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionDeploiement_, asnMission.position_deploiement );

    delete &positionDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_SeDeployer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_SeDeployer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_log_se_deployer ());
    Mission_Automate_LOG_SeDeployer& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_log_se_deployer;

    ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




