// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
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
    : Mission_Automat_ABC ( "Automat_ASA_DefendreItineraire", Automat )
{
    // NOTHING
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

    Mission_Automate_ASA_DefendreItineraire& asnMission = *new Mission_Automate_ASA_DefendreItineraire();

    asnMsg_.GetAsnMsg().mission().mission_automate_asa_defendre_itineraire = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );

    delete &itineraire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreItineraire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_DefendreItineraire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_asa_defendre_itineraire ());
    Mission_Automate_ASA_DefendreItineraire& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_asa_defendre_itineraire;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



