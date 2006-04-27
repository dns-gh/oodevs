// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_INF_AppuyerUneAttaque.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUneAttaque constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AppuyerUneAttaque::Mission_Automat_INF_AppuyerUneAttaque( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_AppuyerUneAttaque", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUneAttaque destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AppuyerUneAttaque::~Mission_Automat_INF_AppuyerUneAttaque()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUneAttaque::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AppuyerUneAttaque::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_AppuyerUneAttaque& asnMission = *new ASN1T_Mission_Automate_INF_AppuyerUneAttaque();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_appuyer_une_attaque;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_appuyer_une_attaque = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUneAttaque::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AppuyerUneAttaque::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_appuyer_une_attaque );
    ASN1T_Mission_Automate_INF_AppuyerUneAttaque& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_appuyer_une_attaque;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




