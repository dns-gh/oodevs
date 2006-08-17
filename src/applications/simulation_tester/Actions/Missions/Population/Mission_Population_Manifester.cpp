// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Population_Manifester.h"
#include "Entities/Population.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Population_Manifester constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Population_Manifester::Mission_Population_Manifester( Population& Population )
    : Mission_Population_ABC ( "Population_Manifester", Population )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_Manifester destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Population_Manifester::~Mission_Population_Manifester()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_Manifester::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Population_Manifester::Serialize()
{
    // build din/asn msg
    Mission_Population_ABC::Serialize();

    ASN1T_Mission_Population_Manifester& asnMission = *new ASN1T_Mission_Population_Manifester();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Population_mission_population_manifester;
    asnMsg_.GetAsnMsg().mission.u.mission_population_manifester = &asnMission;

    const Position& destination_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( destination_, asnMission.destination );

    delete &destination_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Population_Manifester::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Population_Manifester::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Population_mission_population_manifester );
    ASN1T_Mission_Population_Manifester& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_population_manifester;

    ASN_Tools::Delete( asnMission.destination );

    delete &asnMission;
    Mission_Population_ABC::Clean();
}




