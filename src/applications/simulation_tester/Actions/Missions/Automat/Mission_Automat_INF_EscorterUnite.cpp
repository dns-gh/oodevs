// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_EscorterUnite.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_EscorterUnite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_EscorterUnite::Mission_Automat_INF_EscorterUnite( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_EscorterUnite", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_EscorterUnite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_EscorterUnite::~Mission_Automat_INF_EscorterUnite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_EscorterUnite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_EscorterUnite::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_INF_EscorterUnite& asnMission = *new Mission_Automate_INF_EscorterUnite();

    asnMsg_.GetAsnMsg().mission().mission_automate_inf_escorter_unite = &asnMission;


    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.unite_a_escorter );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_EscorterUnite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_EscorterUnite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_inf_escorter_unite ());
    Mission_Automate_INF_EscorterUnite& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_inf_escorter_unite;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




