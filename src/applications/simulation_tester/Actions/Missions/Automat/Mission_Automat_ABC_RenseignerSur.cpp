// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_RenseignerSur.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RenseignerSur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_RenseignerSur::Mission_Automat_ABC_RenseignerSur( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_RenseignerSur", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RenseignerSur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_RenseignerSur::~Mission_Automat_ABC_RenseignerSur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RenseignerSur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_RenseignerSur::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_RenseignerSur& asnMission = *new ASN1T_Mission_Automate_ABC_RenseignerSur();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_renseigner_sur;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_renseigner_sur = &asnMission;

    const Location& zoneARenseigner_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneARenseigner_, asnMission.zone_a_renseigner );

    delete &zoneARenseigner_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RenseignerSur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_RenseignerSur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_renseigner_sur );
    ASN1T_Mission_Automate_ABC_RenseignerSur& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_renseigner_sur;

    ASN_Tools::Delete( asnMission.zone_a_renseigner );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




