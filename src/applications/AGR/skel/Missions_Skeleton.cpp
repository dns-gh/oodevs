// *****************************************************************************
//
// $Created: AGE 2004-09-16 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/Missions_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 14:20 $
// $Revision: 5 $
// $Workfile: Missions_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "Missions.h"

#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"

$IncludeList$

using namespace missions;

//-----------------------------------------------------------------------------
// Name: RegisterAutomateMissions
// Created: AGR 
//-----------------------------------------------------------------------------
void missions::RegisterAutomateMissions()
{
$AutomateMissionsRegistration$
}

// -----------------------------------------------------------------------------
// Name: RegisterPionMissions
// Created: AGR 
// -----------------------------------------------------------------------------
void missions::RegisterPionMissions()
{
$PionMissionsRegistration$
}

// -----------------------------------------------------------------------------
// Name: RegisterOrderConduites
// Created: AGR 
// -----------------------------------------------------------------------------
void missions::RegisterOrderConduites()
{
$OrderConduiteRegistration$
}
