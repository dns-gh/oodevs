// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_FaireMouvementVersCampRefugies.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvementVersCampRefugies constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_FaireMouvementVersCampRefugies::Mission_Automat_FaireMouvementVersCampRefugies( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_FaireMouvementVersCampRefugies", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvementVersCampRefugies destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_FaireMouvementVersCampRefugies::~Mission_Automat_FaireMouvementVersCampRefugies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvementVersCampRefugies::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_FaireMouvementVersCampRefugies::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_FaireMouvementVersCampRefugies& asnMission = *new Mission_Automate_FaireMouvementVersCampRefugies();

    asnMsg_.GetAsnMsg().mission().mission_automate_faire_mouvement_vers_camp_refugies = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvementVersCampRefugies::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_FaireMouvementVersCampRefugies::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_faire_mouvement_vers_camp_refugies ());
    Mission_Automate_FaireMouvementVersCampRefugies& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_faire_mouvement_vers_camp_refugies;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



