// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_Semparer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Semparer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Semparer::Mission_Automat_ABC_Semparer( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Semparer", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Semparer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Semparer::~Mission_Automat_ABC_Semparer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Semparer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Semparer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_Semparer& asnMission = *new ASN1T_Mission_Automate_ABC_Semparer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_semparer;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_semparer = &asnMission;

    const Location& objectif_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( objectif_, asnMission.objectif );

    delete &objectif_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Semparer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Semparer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_semparer );
    ASN1T_Mission_Automate_ABC_Semparer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_semparer;

    ASN_Tools::Delete( asnMission.objectif );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




