// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_NBC_DecontaminerUneZone.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_DecontaminerUneZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_DecontaminerUneZone::Mission_Automat_NBC_DecontaminerUneZone( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_NBC_DecontaminerUneZone", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_DecontaminerUneZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_DecontaminerUneZone::~Mission_Automat_NBC_DecontaminerUneZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_DecontaminerUneZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_DecontaminerUneZone::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_NBC_DecontaminerUneZone& asnMission = *new Mission_Automate_NBC_DecontaminerUneZone();

    asnMsg_.GetAsnMsg().mission().mission_automate_nbc_decontaminer_une_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_DecontaminerUneZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_DecontaminerUneZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_nbc_decontaminer_une_zone ());
    Mission_Automate_NBC_DecontaminerUneZone& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_nbc_decontaminer_une_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




