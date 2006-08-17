// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_SeRecompleter.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeRecompleter constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SeRecompleter::Mission_Automat_SeRecompleter( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_SeRecompleter", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeRecompleter destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SeRecompleter::~Mission_Automat_SeRecompleter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeRecompleter::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_SeRecompleter::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_SeRecompleter& asnMission = *new ASN1T_Mission_Automate_SeRecompleter();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_se_recompleter;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_se_recompleter = &asnMission;

    const Location& zoneRavitaillement_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneRavitaillement_, asnMission.zone_ravitaillement );

    delete &zoneRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeRecompleter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_SeRecompleter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_se_recompleter );
    ASN1T_Mission_Automate_SeRecompleter& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_se_recompleter;

    ASN_Tools::Delete( asnMission.zone_ravitaillement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




