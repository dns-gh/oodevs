// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_$MissionName$.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_$MissionName$ constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_$MissionName$::Mission_Pawn_$MissionName$( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_$MissionName$", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_$MissionName$ destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_$MissionName$::~Mission_Pawn_$MissionName$()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_$MissionName$::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_$MissionName$::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_$MissionName$& asnMission = *new ASN1T_Mission_Pion_$MissionName$();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_$LowerMissionName$;
    asnMsg_.GetAsnMsg().mission.u.$LowerMissionName$ = &asnMission;

$DynamicParamAllocation$
$SerializeMembers$
$DynamicParamCleaning$
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_$MissionName$::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_$MissionName$::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_$LowerMissionName$ );
    ASN1T_Mission_Pion_$MissionName$& asnMission = *asnMsg_.GetAsnMsg().mission.u.$LowerMissionName$;

$CleanSerializedMembers$
    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


