// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_SeFaireTransporter.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireTransporter constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SeFaireTransporter::Mission_Automat_SeFaireTransporter( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_SeFaireTransporter", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireTransporter destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SeFaireTransporter::~Mission_Automat_SeFaireTransporter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireTransporter::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_SeFaireTransporter::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_SeFaireTransporter& asnMission = *new Mission_Automate_SeFaireTransporter();

    asnMsg_.GetAsnMsg().mission().mission_automate_se_faire_transporter = &asnMission;

    const Location& zoneEmbarquement_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneEmbarquement_, asnMission.zone_embarquement );

    delete &zoneEmbarquement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireTransporter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_SeFaireTransporter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_se_faire_transporter ());
    Mission_Automate_SeFaireTransporter& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_se_faire_transporter;

    ASN_Tools::Delete( asnMission.zone_embarquement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




