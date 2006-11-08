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
#include "MIL_PionMission_GEN_ContournerObstacles.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_ContournerObstacles::nDIAObstaclesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ContournerObstacles::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_ContournerObstacles::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAObstaclesIdx_ = DEC_Tools::InitializeDIAField( "obstacles_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ContournerObstacles constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_ContournerObstacles::MIL_PionMission_GEN_ContournerObstacles( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ContournerObstacles destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_ContournerObstacles::~MIL_PionMission_GEN_ContournerObstacles()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ContournerObstacles::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_ContournerObstacles::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_ContournerObstacles& asnMission = *asnMsg.mission.u.mission_pion_gen_contourner_obstacles;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.obstacles, GetVariable( nDIAObstaclesIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ContournerObstacles::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_ContournerObstacles::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ContournerObstacles::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_ContournerObstacles::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_ContournerObstacles& mission = static_cast< MIL_PionMission_GEN_ContournerObstacles& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledgeList( mission.GetVariable( nDIAObstaclesIdx_ ), GetVariable( nDIAObstaclesIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ContournerObstacles::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ContournerObstacles::Terminate()
{
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ContournerObstacles::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ContournerObstacles::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_ContournerObstacles& asnMission = *new ASN1T_Mission_Pion_GEN_ContournerObstacles();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_contourner_obstacles;
    asnMsg.mission.u.mission_pion_gen_contourner_obstacles  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAObstaclesIdx_ ), asnMission.obstacles, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ContournerObstacles::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ContournerObstacles::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_contourner_obstacles );
    ASN1T_Mission_Pion_GEN_ContournerObstacles& asnMission = *asnMsg.mission.u.mission_pion_gen_contourner_obstacles;

    NET_ASN_Tools::Delete( asnMission.obstacles );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
