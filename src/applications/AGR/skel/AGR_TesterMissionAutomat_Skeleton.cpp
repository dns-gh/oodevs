// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_$MissionName$.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_$MissionName$ constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_$MissionName$::Mission_Automat_$MissionName$( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_$MissionName$", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_$MissionName$ destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_$MissionName$::~Mission_Automat_$MissionName$()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_$MissionName$::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_$MissionName$::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_$MissionName$& asnMission = *new ASN1T_Mission_Automate_$MissionName$();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_$LowerMissionName$;
    asnMsg_.GetAsnMsg().mission.u.$LowerMissionName$ = &asnMission;

$DynamicParamAllocation$
$SerializeMembers$
$DynamicParamCleaning$
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_$MissionName$::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_$MissionName$::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_$LowerMissionName$ );
    ASN1T_Mission_Automate_$MissionName$& asnMission = *asnMsg_.GetAsnMsg().mission.u.$LowerMissionName$;

$CleanSerializedMembers$
    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




