// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_Attaquer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Attaquer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Attaquer::Mission_Automat_ABC_Attaquer( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Attaquer", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Attaquer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Attaquer::~Mission_Automat_ABC_Attaquer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Attaquer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Attaquer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_Attaquer& asnMission = *new Mission_Automate_ABC_Attaquer();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_attaquer = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Attaquer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Attaquer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_attaquer ());
    Mission_Automate_ABC_Attaquer& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_attaquer;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



