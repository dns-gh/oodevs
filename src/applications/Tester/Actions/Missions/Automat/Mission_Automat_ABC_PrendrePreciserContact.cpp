// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_ABC_PrendrePreciserContact.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_PrendrePreciserContact constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_PrendrePreciserContact::Mission_Automat_ABC_PrendrePreciserContact( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_PrendrePreciserContact", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_PrendrePreciserContact destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_PrendrePreciserContact::~Mission_Automat_ABC_PrendrePreciserContact()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_PrendrePreciserContact::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_PrendrePreciserContact::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_PrendrePreciserContact& asnMission = *new ASN1T_Mission_Automate_ABC_PrendrePreciserContact();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_prendre_preciser_contact;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_prendre_preciser_contact = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_PrendrePreciserContact::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_PrendrePreciserContact::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_prendre_preciser_contact );
    ASN1T_Mission_Automate_ABC_PrendrePreciserContact& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_prendre_preciser_contact;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




