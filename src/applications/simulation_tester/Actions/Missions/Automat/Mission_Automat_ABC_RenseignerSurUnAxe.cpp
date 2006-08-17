// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_RenseignerSurUnAxe.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RenseignerSurUnAxe constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_RenseignerSurUnAxe::Mission_Automat_ABC_RenseignerSurUnAxe( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_RenseignerSurUnAxe", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RenseignerSurUnAxe destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_RenseignerSurUnAxe::~Mission_Automat_ABC_RenseignerSurUnAxe()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RenseignerSurUnAxe::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_RenseignerSurUnAxe::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe& asnMission = *new ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_renseigner_sur_un_axe;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_renseigner_sur_un_axe = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_RenseignerSurUnAxe::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_RenseignerSurUnAxe::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_renseigner_sur_un_axe );
    ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_renseigner_sur_un_axe;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




