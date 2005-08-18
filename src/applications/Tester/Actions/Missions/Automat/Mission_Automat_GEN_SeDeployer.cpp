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
#include "Mission_Automat_GEN_SeDeployer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_SeDeployer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_SeDeployer::Mission_Automat_GEN_SeDeployer( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_GEN_SeDeployer";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_SeDeployer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_SeDeployer::~Mission_Automat_GEN_SeDeployer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_SeDeployer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_SeDeployer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_SeDeployer& asnMission = *new ASN1T_Mission_Automate_GEN_SeDeployer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_se_deployer;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_se_deployer = &asnMission;

    const Location& zoneDeploiement_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneDeploiement_, asnMission.zone_deploiement );

    delete &zoneDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_SeDeployer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_SeDeployer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_se_deployer );
    ASN1T_Mission_Automate_GEN_SeDeployer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_se_deployer;

    ASN_Tools::Delete( asnMission.zone_deploiement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




