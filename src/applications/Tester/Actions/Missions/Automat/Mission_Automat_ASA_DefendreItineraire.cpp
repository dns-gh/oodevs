// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "Mission_Automat_ASA_DefendreItineraire.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreItineraire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_DefendreItineraire::Mission_Automat_ASA_DefendreItineraire( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ASA_DefendreItineraire";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreItineraire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_DefendreItineraire::~Mission_Automat_ASA_DefendreItineraire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreItineraire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_DefendreItineraire::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ASA_DefendreItineraire& asnMission = *new ASN1T_Mission_Automate_ASA_DefendreItineraire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_defendre_itineraire;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_asa_defendre_itineraire = &asnMission;

    ASN_Tools::CopyPath( pTarget_->GetTestParam_Path(), asnMission.itineraire );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreItineraire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_DefendreItineraire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_asa_defendre_itineraire );
    ASN1T_Mission_Automate_ASA_DefendreItineraire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_asa_defendre_itineraire;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




