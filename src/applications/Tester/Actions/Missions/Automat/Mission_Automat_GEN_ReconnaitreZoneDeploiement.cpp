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
#include "Mission_Automat_GEN_ReconnaitreZoneDeploiement.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_ReconnaitreZoneDeploiement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_ReconnaitreZoneDeploiement::Mission_Automat_GEN_ReconnaitreZoneDeploiement( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_GEN_ReconnaitreZoneDeploiement";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_ReconnaitreZoneDeploiement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_ReconnaitreZoneDeploiement::~Mission_Automat_GEN_ReconnaitreZoneDeploiement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_ReconnaitreZoneDeploiement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_ReconnaitreZoneDeploiement::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement& asnMission = *new ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_reconnaitre_zone_deploiement;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_reconnaitre_zone_deploiement = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_ReconnaitreZoneDeploiement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_ReconnaitreZoneDeploiement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_reconnaitre_zone_deploiement );
    ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_reconnaitre_zone_deploiement;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




