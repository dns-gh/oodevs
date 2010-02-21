// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ReleverSurPlace.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ReleverSurPlace constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ReleverSurPlace::Mission_Automat_ReleverSurPlace( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ReleverSurPlace", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ReleverSurPlace destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ReleverSurPlace::~Mission_Automat_ReleverSurPlace()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ReleverSurPlace::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ReleverSurPlace::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ReleverSurPlace& asnMission = *new Mission_Automate_ReleverSurPlace();

    asnMsg_.GetAsnMsg().mission().mission_automate_relever_sur_place = &asnMission;

    const Location& zoneInstallation_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie_a_relever );
    ASN_Tools::CopyPolygon( zoneInstallation_, asnMission.zone_installation );

    delete &zoneInstallation_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ReleverSurPlace::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ReleverSurPlace::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_relever_sur_place ());
    Mission_Automate_ReleverSurPlace& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_relever_sur_place;

    ASN_Tools::Delete( asnMission.zone_installation );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




