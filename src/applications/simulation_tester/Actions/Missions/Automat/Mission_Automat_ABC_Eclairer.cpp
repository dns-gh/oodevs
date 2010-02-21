// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_Eclairer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Eclairer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Eclairer::Mission_Automat_ABC_Eclairer( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Eclairer", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Eclairer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Eclairer::~Mission_Automat_ABC_Eclairer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Eclairer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Eclairer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_Eclairer& asnMission = *new Mission_Automate_ABC_Eclairer();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_eclairer = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Eclairer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Eclairer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_eclairer ());
    Mission_Automate_ABC_Eclairer& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_eclairer;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




