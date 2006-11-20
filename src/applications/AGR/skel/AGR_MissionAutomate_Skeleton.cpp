// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"

#include "MIL_AutomateMission_$MissionName$.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

$StaticMemberInit$

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_$MissionName$ constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_$MissionName$::MIL_AutomateMission_$MissionName$( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_$MissionName$ destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_$MissionName$::~MIL_AutomateMission_$MissionName$()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_$MissionName$::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_$MissionName$::InitializeDIA( const MIL_AutomateMissionType& type )
{
$StaticMemberScriptInit$
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_$MissionName$::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_$MissionName$::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

$InitMembersFromAsn$
    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_$MissionName$::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_$MissionName$::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_$MissionName$& asnMission = *new ASN1T_Mission_Automate_$MissionName$();
    asnMsg.mission.t                           = T_Mission_Automate_$LowerMissionName$;
    asnMsg.mission.u.$LowerMissionName$  = &asnMission;

$SerializeMembers$
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_$MissionName$::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_$MissionName$::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_$LowerMissionName$ );
    ASN1T_Mission_Automate_$MissionName$& asnMission = *asnMsg.mission.u.$LowerMissionName$;

$CleanSerializedMembers$
    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
