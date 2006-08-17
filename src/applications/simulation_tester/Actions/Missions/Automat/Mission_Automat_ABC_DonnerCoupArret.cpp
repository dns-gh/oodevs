// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_DonnerCoupArret.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_DonnerCoupArret constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_DonnerCoupArret::Mission_Automat_ABC_DonnerCoupArret( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_DonnerCoupArret", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_DonnerCoupArret destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_DonnerCoupArret::~Mission_Automat_ABC_DonnerCoupArret()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_DonnerCoupArret::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_DonnerCoupArret::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_DonnerCoupArret& asnMission = *new ASN1T_Mission_Automate_ABC_DonnerCoupArret();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_donner_coup_arret;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_donner_coup_arret = &asnMission;

    const Location& zoneRegroupement_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneRegroupement_, asnMission.zone_regroupement );

    delete &zoneRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_DonnerCoupArret::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_DonnerCoupArret::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_donner_coup_arret );
    ASN1T_Mission_Automate_ABC_DonnerCoupArret& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_donner_coup_arret;

    ASN_Tools::Delete( asnMission.zone_regroupement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




