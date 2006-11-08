// *****************************************************************************
//
// $Created: 
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPopulation_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPopulation_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"
#include "MIL_PopulationMission_$MissionName$.h"

#include "simulation_kernel/Entities/Orders/Population/MIL_PopulationOrderManager.h"
#include "simulation_kernel/Entities/Orders/Population/MIL_PopulationMissionType.h"
#include "simulation_kernel/Entities/Populations/MIL_Population.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

$StaticMemberInit$

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_$MissionName$::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PopulationMission_$MissionName$::InitializeDIA( const MIL_PopulationMissionType& type )
{
$StaticMemberScriptInit$
}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_$MissionName$ constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PopulationMission_$MissionName$::MIL_PopulationMission_$MissionName$( MIL_Population& population, const MIL_PopulationMissionType& type )
: MIL_PopulationMission_ABC( population, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_$MissionName$ destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PopulationMission_$MissionName$::~MIL_PopulationMission_$MissionName$()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_$MissionName$::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PopulationMission_$MissionName$::Initialize( const ASN1T_MsgPopulationOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PopulationMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

$InitMembersFromAsn$
    return EnumOrderErrorCode::no_error;
}

/*
// ------------------------------------------------------------------------------
// Name: MIL_PopulationMission_$MissionName$::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PopulationMission_$MissionName$::Initialize( MIL_PopulationMission_ABC& missionTmp )
{
    if( !MIL_PopulationMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PopulationMission_$MissionName$& mission = static_cast< MIL_PopulationMission_$MissionName$& >( missionTmp );

$InitMemberFromMission$
    return true;
}                                                                    
*/
//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_$MissionName$::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PopulationMission_$MissionName$::Terminate()
{
    MIL_PopulationMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

/*
//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_$MissionName$::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PopulationMission_$MissionName$::Serialize( ASN1T_MsgPopulationOrder& asnMsg )
{
    MIL_PopulationMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Population_$MissionName$& asnMission = *new ASN1T_Mission_Population_$MissionName$();
    asnMsg.mission.t                           = T_Mission_Population_$LowerMissionName$;
    asnMsg.mission.u.$LowerMissionName$  = &asnMission;

$SerializeMembers$
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_$MissionName$::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PopulationMission_$MissionName$::CleanAfterSerialization( ASN1T_MsgPopulationOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Population_$LowerMissionName$ );
    ASN1T_Mission_Population_$MissionName$& asnMission = *asnMsg.mission.u.$LowerMissionName$;

$CleanSerializedMembers$
    delete &asnMission;

    MIL_PopulationMission_ABC::CleanAfterSerialization( asnMsg );
}
*/