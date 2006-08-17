// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_MenerDefenseUsure.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_MenerDefenseUsure constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_MenerDefenseUsure::Mission_Automat_INF_MenerDefenseUsure( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_MenerDefenseUsure", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_MenerDefenseUsure destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_MenerDefenseUsure::~Mission_Automat_INF_MenerDefenseUsure()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_MenerDefenseUsure::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_MenerDefenseUsure::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_MenerDefenseUsure& asnMission = *new ASN1T_Mission_Automate_INF_MenerDefenseUsure();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_mener_defense_usure;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_mener_defense_usure = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_MenerDefenseUsure::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_MenerDefenseUsure::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_mener_defense_usure );
    ASN1T_Mission_Automate_INF_MenerDefenseUsure& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_mener_defense_usure;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




