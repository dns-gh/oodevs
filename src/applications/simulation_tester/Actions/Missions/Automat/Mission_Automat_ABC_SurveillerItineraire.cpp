// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_SurveillerItineraire.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_SurveillerItineraire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_SurveillerItineraire::Mission_Automat_ABC_SurveillerItineraire( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_SurveillerItineraire", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_SurveillerItineraire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_SurveillerItineraire::~Mission_Automat_ABC_SurveillerItineraire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_SurveillerItineraire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_SurveillerItineraire::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_SurveillerItineraire& asnMission = *new Mission_Automate_ABC_SurveillerItineraire();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_surveiller_itineraire = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );

    delete &itineraire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_SurveillerItineraire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_SurveillerItineraire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_surveiller_itineraire ());
    Mission_Automate_ABC_SurveillerItineraire& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_surveiller_itineraire;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




