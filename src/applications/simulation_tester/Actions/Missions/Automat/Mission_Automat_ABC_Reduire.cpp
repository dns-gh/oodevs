// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_Reduire.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reduire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Reduire::Mission_Automat_ABC_Reduire( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Reduire", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reduire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Reduire::~Mission_Automat_ABC_Reduire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reduire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Reduire::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_Reduire& asnMission = *new ASN1T_Mission_Automate_ABC_Reduire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_reduire;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_reduire = &asnMission;

    const Location& zoneResistance_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneResistance_, asnMission.zone_resistance );

    delete &zoneResistance_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reduire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Reduire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_reduire );
    ASN1T_Mission_Automate_ABC_Reduire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_reduire;

    ASN_Tools::Delete( asnMission.zone_resistance );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




