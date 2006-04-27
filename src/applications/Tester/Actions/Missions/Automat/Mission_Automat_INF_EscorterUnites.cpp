// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_INF_EscorterUnites.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_EscorterUnites constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_EscorterUnites::Mission_Automat_INF_EscorterUnites( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_EscorterUnites", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_EscorterUnites destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_EscorterUnites::~Mission_Automat_INF_EscorterUnites()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_EscorterUnites::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_EscorterUnites::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_EscorterUnites& asnMission = *new ASN1T_Mission_Automate_INF_EscorterUnites();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_escorter_unites;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_escorter_unites = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.automate_a_escorter );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_EscorterUnites::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_EscorterUnites::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_escorter_unites );
    ASN1T_Mission_Automate_INF_EscorterUnites& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_escorter_unites;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




