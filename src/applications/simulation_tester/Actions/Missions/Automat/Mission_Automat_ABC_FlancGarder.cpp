// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_FlancGarder.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_FlancGarder constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_FlancGarder::Mission_Automat_ABC_FlancGarder( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_FlancGarder", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_FlancGarder destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_FlancGarder::~Mission_Automat_ABC_FlancGarder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_FlancGarder::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_FlancGarder::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_FlancGarder& asnMission = *new Mission_Automate_ABC_FlancGarder();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_flanc_garder = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_FlancGarder::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_FlancGarder::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_flanc_garder ());
    Mission_Automate_ABC_FlancGarder& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_flanc_garder;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




