// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_ABC_RecueillirUnite.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RecueillirUnite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_RecueillirUnite::Mission_Automat_ABC_RecueillirUnite( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_RecueillirUnite", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RecueillirUnite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_RecueillirUnite::~Mission_Automat_ABC_RecueillirUnite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RecueillirUnite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_RecueillirUnite::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_RecueillirUnite& asnMission = *new ASN1T_Mission_Automate_ABC_RecueillirUnite();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_recueillir_unite;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_recueillir_unite = &asnMission;

    const Location& zoneRegroupement_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneRegroupement_, asnMission.zone_regroupement );

    delete &zoneRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RecueillirUnite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_RecueillirUnite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_recueillir_unite );
    ASN1T_Mission_Automate_ABC_RecueillirUnite& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_recueillir_unite;

    ASN_Tools::Delete( asnMission.zone_regroupement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




