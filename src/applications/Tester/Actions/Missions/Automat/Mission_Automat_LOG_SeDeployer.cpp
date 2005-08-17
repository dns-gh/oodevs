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
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_LOG_SeDeployer";
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

    ASN1T_Mission_Automate_LOG_SeDeployer& asnMission = *new ASN1T_Mission_Automate_LOG_SeDeployer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_se_deployer;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_log_se_deployer = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.position_deploiement );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_SeDeployer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_SeDeployer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_log_se_deployer );
    ASN1T_Mission_Automate_LOG_SeDeployer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_log_se_deployer;

    ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




