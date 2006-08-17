// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_Freiner.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Freiner constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Freiner::Mission_Automat_INF_Freiner( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_Freiner", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Freiner destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Freiner::~Mission_Automat_INF_Freiner()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Freiner::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Freiner::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_Freiner& asnMission = *new ASN1T_Mission_Automate_INF_Freiner();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_freiner;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_freiner = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Freiner::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Freiner::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_freiner );
    ASN1T_Mission_Automate_INF_Freiner& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_freiner;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




