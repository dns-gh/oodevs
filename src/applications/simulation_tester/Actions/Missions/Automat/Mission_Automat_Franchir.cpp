// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_Franchir.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Franchir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Franchir::Mission_Automat_Franchir( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_Franchir", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Franchir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Franchir::~Mission_Automat_Franchir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Franchir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_Franchir::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_Franchir& asnMission = *new Mission_Automate_Franchir();

    asnMsg_.GetAsnMsg().mission().mission_automate_franchir = &asnMission;

    const Location& zoneArrivee_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.pont_flottant );
    ASN_Tools::CopyPolygon( zoneArrivee_, asnMission.zone_arrivee );

    delete &zoneArrivee_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Franchir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_Franchir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_franchir ());
    Mission_Automate_Franchir& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_franchir;

    ASN_Tools::Delete( asnMission.zone_arrivee );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



