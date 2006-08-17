// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_Semparer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Semparer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Semparer::Mission_Automat_INF_Semparer( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_Semparer", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Semparer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Semparer::~Mission_Automat_INF_Semparer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Semparer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Semparer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_Semparer& asnMission = *new ASN1T_Mission_Automate_INF_Semparer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_semparer;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_semparer = &asnMission;

    const Location& objectif_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( objectif_, asnMission.objectif );

    delete &objectif_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Semparer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Semparer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_semparer );
    ASN1T_Mission_Automate_INF_Semparer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_semparer;

    ASN_Tools::Delete( asnMission.objectif );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




