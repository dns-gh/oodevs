// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_AppuyerUnFreinage.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUnFreinage constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AppuyerUnFreinage::Mission_Automat_INF_AppuyerUnFreinage( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_AppuyerUnFreinage", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUnFreinage destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AppuyerUnFreinage::~Mission_Automat_INF_AppuyerUnFreinage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUnFreinage::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AppuyerUnFreinage::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_INF_AppuyerUnFreinage& asnMission = *new Mission_Automate_INF_AppuyerUnFreinage();

    asnMsg_.GetAsnMsg().mission().mission_automate_inf_appuyer_un_freinage = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUnFreinage::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AppuyerUnFreinage::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_inf_appuyer_un_freinage ());
    Mission_Automate_INF_AppuyerUnFreinage& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_inf_appuyer_un_freinage;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




