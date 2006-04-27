// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_INF_PrendrePreciserMaintenirContact.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_PrendrePreciserMaintenirContact constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_PrendrePreciserMaintenirContact::Mission_Automat_INF_PrendrePreciserMaintenirContact( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_PrendrePreciserMaintenirContact", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_PrendrePreciserMaintenirContact destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_PrendrePreciserMaintenirContact::~Mission_Automat_INF_PrendrePreciserMaintenirContact()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_PrendrePreciserMaintenirContact::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_PrendrePreciserMaintenirContact::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact& asnMission = *new ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_prendre_preciser_maintenir_contact;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_prendre_preciser_maintenir_contact = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_PrendrePreciserMaintenirContact::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_PrendrePreciserMaintenirContact::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_prendre_preciser_maintenir_contact );
    ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_prendre_preciser_maintenir_contact;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




