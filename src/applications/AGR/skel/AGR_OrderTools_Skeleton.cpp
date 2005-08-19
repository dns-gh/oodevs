// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $TIME$ $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderTools_Skeleton.cpp $
// $Author: Agn $
// $Modtime: 28/04/04 14:10 $
// $Revision: 2 $
// $Workfile: AGR_OrderTools_Skeleton.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "MIL_OrderTools.h"

#ifndef USE_INLINE
#   include "MIL_OrderTools.inl"
#endif

#include "Order/Pion/MIL_PionMission_ABC.h"

#include "Order/Pion/MIL_PionMissionTest_MoveTo.h"
#include "Order/Pion/MIL_PionMissionTest_CreateObject.h"
#include "Order/Pion/MIL_PionMissionTest_DestroyObject.h"
#include "Order/Pion/MIL_PionMissionTest_CreateBypass.h"
#include "Order/Pion/MIL_PionMissionTest_Fire.h"
#include "Order/Pion/MIL_PionMissionTest_ChangePosture.h"
#include "Order/Pion/MIL_PionMissionTest_FindPosition.h"
#include "Order/Pion/MIL_PionMissionTest_PathFind.h"
#include "Order/Pion/MIL_PionMissionTest_Renfort.h"
$IncludePionMissionHeaders$

#include "Order/Automate/MIL_AutomateMission_ABC.h"

#include "Order/Automate/MIL_AutomateMissionTest_MoveTo.h"
$IncludeAutomateMissionHeaders$


//-----------------------------------------------------------------------------
// Name: MIL_OrderTools::GetFreePionMission
// Created: $TIME$
//-----------------------------------------------------------------------------
// static
MIL_PionMission_ABC* MIL_OrderTools::GetFreePionMission( E_MissionID nMissionID )
{
    switch( nMissionID )
    {
		case eMission_Pion_Test_MoveTo: return new MIL_PionMissionTest_MoveTo();
		case eMission_Pion_Test_CreateObject: return new MIL_PionMissionTest_CreateObject();
		case eMission_Pion_Test_DestroyObject: return new MIL_PionMissionTest_DestroyObject();
		case eMission_Pion_Test_CreateBypass: return new MIL_PionMissionTest_CreateBypass();
		case eMission_Pion_Test_Fire: return new MIL_PionMissionTest_Fire();
		case eMission_Pion_Test_ChangePosture: return new MIL_PionMissionTest_ChangePosture();
		case eMission_Pion_Test_FindPosition: return new MIL_PionMissionTest_FindPosition();
		case eMission_Pion_Test_PathFind: return new MIL_PionMissionTest_PathFind();
		case eMission_Pion_Test_Reinforce: return new MIL_PionMissionTest_Renfort();
$CreatePionMission$
        default:
            return 0;
    }
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderTools::GetFreeAutomateMission
// Created: $TIME$
//-----------------------------------------------------------------------------
// static
MIL_AutomateMission_ABC* MIL_OrderTools::GetFreeAutomateMission( E_MissionID nMissionID )
{
    switch( nMissionID )
    {
		case eMission_Automate_Test_MoveTo: return new MIL_AutomateMissionTest_MoveTo();
$CreateAutomateMission$
        default:
            return 0;
    }
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderTools::RecycleMission
// Created: $TIME$
//-----------------------------------------------------------------------------
// static
void MIL_OrderTools::RecycleMission( MIL_PionMission_ABC& mission )
{
    //$$$ TMP
    delete &mission;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderTools::RecycleMission
// Created: $TIME$
//-----------------------------------------------------------------------------
// static
void MIL_OrderTools::RecycleMission( MIL_AutomateMission_ABC& mission )
{
    //$$$ TMP
    delete &mission;
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderTools::InitializeDIA
// Created: $TIME$
//-----------------------------------------------------------------------------
// static
void MIL_OrderTools::InitializeDIA()
{
    // Missions pion
    MIL_PionMission_ABC::InitializeDIA();
    MIL_AutomateMission_ABC::InitializeDIA();
	MIL_PionMissionTest_MoveTo::InitializeDIA();
	MIL_PionMissionTest_CreateObject::InitializeDIA();
	MIL_PionMissionTest_DestroyObject::InitializeDIA();
	MIL_PionMissionTest_CreateBypass::InitializeDIA();
	MIL_PionMissionTest_Fire::InitializeDIA();
	MIL_PionMissionTest_ChangePosture::InitializeDIA();
	MIL_PionMissionTest_FindPosition::InitializeDIA();
	MIL_PionMissionTest_PathFind::InitializeDIA();
	MIL_PionMissionTest_Renfort::InitializeDIA();
	MIL_AutomateMissionTest_MoveTo::InitializeDIA();

$InitMissionDiaIdx$
}

//=============================================================================
// CONVERSION TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_OrderTools::ConvertMissionIDAsnToSim
// Created: $TIME$
//-----------------------------------------------------------------------------
// static
E_MissionID MIL_OrderTools::ConvertMissionIDAsnToSim( const ASN1T_Mission_Pion& asnMission )
{
	switch( asnMission.t )
	{
$ConvertPionIdAsnToSim$
		default:
			return eNbrMission;
	}
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderTools::ConvertMissionIDAsnToSim
// Created: $TIME$
//-----------------------------------------------------------------------------
E_MissionID MIL_OrderTools::ConvertMissionIDAsnToSim( const ASN1T_Mission_Automate& asnMission )
{
    switch( asnMission.t )
	{
$ConvertAutomateIdAsnToSim$

        default:
            return eNbrMission;
    }
}

