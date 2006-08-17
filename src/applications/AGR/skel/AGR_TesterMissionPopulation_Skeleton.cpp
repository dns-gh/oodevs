// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Population_$MissionName$.h"
#include "Entities/Population.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Population_$MissionName$ constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Population_$MissionName$::Mission_Population_$MissionName$( Population& Population )
    : Mission_Population_ABC ( "Population_$MissionName$", Population )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_$MissionName$ destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Population_$MissionName$::~Mission_Population_$MissionName$()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_$MissionName$::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Population_$MissionName$::Serialize()
{
    // build din/asn msg
    Mission_Population_ABC::Serialize();

    ASN1T_Mission_Population_$MissionName$& asnMission = *new ASN1T_Mission_Population_$MissionName$();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Population_$LowerMissionName$;
    asnMsg_.GetAsnMsg().mission.u.$LowerMissionName$ = &asnMission;

$DynamicParamAllocation$
$SerializeMembers$
$DynamicParamCleaning$
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_$MissionName$::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Population_$MissionName$::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Population_$LowerMissionName$ );
    ASN1T_Mission_Population_$MissionName$& asnMission = *asnMsg_.GetAsnMsg().mission.u.$LowerMissionName$;

$CleanSerializedMembers$
    delete &asnMission;
    Mission_Population_ABC::Clean();
}




