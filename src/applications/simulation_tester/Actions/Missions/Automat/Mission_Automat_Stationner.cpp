// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_Stationner.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Stationner constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Stationner::Mission_Automat_Stationner( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_Stationner", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Stationner destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Stationner::~Mission_Automat_Stationner()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Stationner::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_Stationner::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_Stationner& asnMission = *new ASN1T_Mission_Automate_Stationner();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_stationner;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_stationner = &asnMission;

    const Location& zoneAttente_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneAttente_, asnMission.zone_attente );

    delete &zoneAttente_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Stationner::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_Stationner::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_stationner );
    ASN1T_Mission_Automate_Stationner& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_stationner;

    ASN_Tools::Delete( asnMission.zone_attente );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




