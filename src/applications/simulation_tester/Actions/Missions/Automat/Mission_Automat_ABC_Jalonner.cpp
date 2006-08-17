// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_Jalonner.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Jalonner constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Jalonner::Mission_Automat_ABC_Jalonner( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Jalonner", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Jalonner destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Jalonner::~Mission_Automat_ABC_Jalonner()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Jalonner::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Jalonner::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_Jalonner& asnMission = *new ASN1T_Mission_Automate_ABC_Jalonner();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_jalonner;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_jalonner = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Jalonner::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Jalonner::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_jalonner );
    ASN1T_Mission_Automate_ABC_Jalonner& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_jalonner;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




