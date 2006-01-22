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

#include "Missions_pch.h"
#include "MIL_PionMission_CanaliserPopulation.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_CanaliserPopulation::nDIAZoneIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulation::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_CanaliserPopulation::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulation constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_CanaliserPopulation::MIL_PionMission_CanaliserPopulation( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulation destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_CanaliserPopulation::~MIL_PionMission_CanaliserPopulation()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulation::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_CanaliserPopulation::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_CanaliserPopulation& asnMission = *asnMsg.mission.u.mission_pion_canaliser_population;
    if( !NET_ASN_Tools::CopyLocation( asnMission.zone, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulation::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_CanaliserPopulation::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulation::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_CanaliserPopulation::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_CanaliserPopulation& mission = static_cast< MIL_PionMission_CanaliserPopulation& >( missionTmp );

    NET_ASN_Tools::CopyLocation( mission.GetVariable( nDIAZoneIdx_ ), GetVariable( nDIAZoneIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulation::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_CanaliserPopulation::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulation::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_CanaliserPopulation::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_CanaliserPopulation& asnMission = *new ASN1T_Mission_Pion_CanaliserPopulation();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_canaliser_population;
    asnMsg.mission.u.mission_pion_canaliser_population  = &asnMission;

    NET_ASN_Tools::CopyLocation( GetVariable( nDIAZoneIdx_ ), asnMission.zone );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulation::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_CanaliserPopulation::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_canaliser_population );
    ASN1T_Mission_Pion_CanaliserPopulation& asnMission = *asnMsg.mission.u.mission_pion_canaliser_population;

    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
