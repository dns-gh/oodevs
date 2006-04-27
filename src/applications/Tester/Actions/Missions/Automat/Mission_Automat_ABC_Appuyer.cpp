// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_ABC_Appuyer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Appuyer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Appuyer::Mission_Automat_ABC_Appuyer( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Appuyer", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Appuyer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Appuyer::~Mission_Automat_ABC_Appuyer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Appuyer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Appuyer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_Appuyer& asnMission = *new ASN1T_Mission_Automate_ABC_Appuyer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_appuyer;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_appuyer = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Appuyer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Appuyer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_appuyer );
    ASN1T_Mission_Automate_ABC_Appuyer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_appuyer;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




