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
#include "MIL_PionMission_CanaliserPopulations.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_CanaliserPopulations::nDIAZoneIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulations::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_CanaliserPopulations::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulations constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_CanaliserPopulations::MIL_PionMission_CanaliserPopulations( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulations destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_CanaliserPopulations::~MIL_PionMission_CanaliserPopulations()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulations::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_CanaliserPopulations::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_CanaliserPopulations& asnMission = *asnMsg.mission.u.mission_pion_canaliser_populations;
    if( !NET_ASN_Tools::CopyLocation( asnMission.zone, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulations::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_CanaliserPopulations::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulations::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_CanaliserPopulations::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_CanaliserPopulations& mission = static_cast< MIL_PionMission_CanaliserPopulations& >( missionTmp );

    NET_ASN_Tools::CopyLocation( mission.GetVariable( nDIAZoneIdx_ ), GetVariable( nDIAZoneIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulations::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_CanaliserPopulations::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulations::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_CanaliserPopulations::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_CanaliserPopulations& asnMission = *new ASN1T_Mission_Pion_CanaliserPopulations();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_canaliser_populations;
    asnMsg.mission.u.mission_pion_canaliser_populations  = &asnMission;

    NET_ASN_Tools::CopyLocation( GetVariable( nDIAZoneIdx_ ), asnMission.zone );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_CanaliserPopulations::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_CanaliserPopulations::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_canaliser_populations );
    ASN1T_Mission_Pion_CanaliserPopulations& asnMission = *asnMsg.mission.u.mission_pion_canaliser_populations;

    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
