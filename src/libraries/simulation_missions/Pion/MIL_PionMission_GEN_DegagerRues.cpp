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
#include "MIL_PionMission_GEN_DegagerRues.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_DegagerRues::nDIAObstacleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DegagerRues::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_DegagerRues::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAObstacleIdx_ = DEC_Tools::InitializeDIAField( "obstacle_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DegagerRues constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_DegagerRues::MIL_PionMission_GEN_DegagerRues( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DegagerRues destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_DegagerRues::~MIL_PionMission_GEN_DegagerRues()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DegagerRues::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_DegagerRues::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_DegagerRues& asnMission = *asnMsg.mission.u.mission_pion_gen_degager_rues;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.obstacle, GetVariable( nDIAObstacleIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DegagerRues::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_DegagerRues::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DegagerRues::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_DegagerRues::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_DegagerRues& mission = static_cast< MIL_PionMission_GEN_DegagerRues& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIAObstacleIdx_ ), GetVariable( nDIAObstacleIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DegagerRues::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DegagerRues::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DegagerRues::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DegagerRues::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_DegagerRues& asnMission = *new ASN1T_Mission_Pion_GEN_DegagerRues();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_degager_rues;
    asnMsg.mission.u.mission_pion_gen_degager_rues  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIAObstacleIdx_ ), asnMission.obstacle, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DegagerRues::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DegagerRues::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_degager_rues );
    ASN1T_Mission_Pion_GEN_DegagerRues& asnMission = *asnMsg.mission.u.mission_pion_gen_degager_rues;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
