// *****************************************************************************
//
// $Created: 
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"
#include "MIL_PionMission_$MissionName$.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

$StaticMemberInit$

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_$MissionName$::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_$MissionName$::InitializeDIA( const MIL_PionMissionType& type )
{
$StaticMemberScriptInit$
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_$MissionName$ constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_$MissionName$::MIL_PionMission_$MissionName$( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_$MissionName$ destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_$MissionName$::~MIL_PionMission_$MissionName$()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_$MissionName$::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_$MissionName$::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

$InitMembersFromAsn$
    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_$MissionName$::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_$MissionName$::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

$InitMembers$
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_$MissionName$::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_$MissionName$::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_$MissionName$& mission = static_cast< MIL_PionMission_$MissionName$& >( missionTmp );

$InitMemberFromMission$
    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_$MissionName$::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_$MissionName$::Terminate()
{
$ResetMembers$
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_$MissionName$::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_$MissionName$::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_$MissionName$& asnMission = *new ASN1T_Mission_Pion_$MissionName$();
    asnMsg.mission.t                           = T_Mission_Pion_$LowerMissionName$;
    asnMsg.mission.u.$LowerMissionName$  = &asnMission;

$SerializeMembers$
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_$MissionName$::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_$MissionName$::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_$LowerMissionName$ );
    ASN1T_Mission_Pion_$MissionName$& asnMission = *asnMsg.mission.u.$LowerMissionName$;

$CleanSerializedMembers$
    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
