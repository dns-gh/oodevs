// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_Reconnaitre.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reconnaitre constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Reconnaitre::Mission_Automat_ABC_Reconnaitre( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Reconnaitre", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reconnaitre destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Reconnaitre::~Mission_Automat_ABC_Reconnaitre()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reconnaitre::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Reconnaitre::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_Reconnaitre& asnMission = *new Mission_Automate_ABC_Reconnaitre();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_reconnaitre = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reconnaitre::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Reconnaitre::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_reconnaitre ());
    Mission_Automate_ABC_Reconnaitre& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_reconnaitre;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




