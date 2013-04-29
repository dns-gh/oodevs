// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_Freiner.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Freiner constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Freiner::Mission_Automat_ABC_Freiner( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Freiner", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Freiner destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Freiner::~Mission_Automat_ABC_Freiner()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Freiner::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Freiner::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_Freiner& asnMission = *new Mission_Automate_ABC_Freiner();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_freiner = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Freiner::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Freiner::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_freiner ());
    Mission_Automate_ABC_Freiner& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_freiner;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



