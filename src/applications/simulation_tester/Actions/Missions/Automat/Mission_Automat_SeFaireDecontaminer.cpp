// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_SeFaireDecontaminer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireDecontaminer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SeFaireDecontaminer::Mission_Automat_SeFaireDecontaminer( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_SeFaireDecontaminer", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireDecontaminer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SeFaireDecontaminer::~Mission_Automat_SeFaireDecontaminer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireDecontaminer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_SeFaireDecontaminer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_SeFaireDecontaminer& asnMission = *new Mission_Automate_SeFaireDecontaminer();

    asnMsg_.GetAsnMsg().mission().mission_automate_se_faire_decontaminer = &asnMission;

    const Location& zoneArrivee_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneArrivee_, asnMission.zone_arrivee );
    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.site_decontamination );

    delete &zoneArrivee_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireDecontaminer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_SeFaireDecontaminer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_se_faire_decontaminer ());
    Mission_Automate_SeFaireDecontaminer& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_se_faire_decontaminer;

    ASN_Tools::Delete( asnMission.zone_arrivee );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




