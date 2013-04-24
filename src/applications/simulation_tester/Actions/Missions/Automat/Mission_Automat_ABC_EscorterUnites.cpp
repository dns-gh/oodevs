// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_EscorterUnites.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnites constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_EscorterUnites::Mission_Automat_ABC_EscorterUnites( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_EscorterUnites", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnites destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_EscorterUnites::~Mission_Automat_ABC_EscorterUnites()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnites::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_EscorterUnites::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_EscorterUnites& asnMission = *new Mission_Automate_ABC_EscorterUnites();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_escorter_unites = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.automate_a_escorter );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnites::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_EscorterUnites::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_escorter_unites ());
    Mission_Automate_ABC_EscorterUnites& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_escorter_unites;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



