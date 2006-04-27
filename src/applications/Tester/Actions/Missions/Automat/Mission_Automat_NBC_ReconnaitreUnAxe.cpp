// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_NBC_ReconnaitreUnAxe.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreUnAxe constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_ReconnaitreUnAxe::Mission_Automat_NBC_ReconnaitreUnAxe( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_NBC_ReconnaitreUnAxe", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreUnAxe destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_ReconnaitreUnAxe::~Mission_Automat_NBC_ReconnaitreUnAxe()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreUnAxe::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_ReconnaitreUnAxe::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe& asnMission = *new ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_nbc_reconnaitre_un_axe;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_un_axe = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreUnAxe::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_ReconnaitreUnAxe::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_nbc_reconnaitre_un_axe );
    ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_un_axe;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




