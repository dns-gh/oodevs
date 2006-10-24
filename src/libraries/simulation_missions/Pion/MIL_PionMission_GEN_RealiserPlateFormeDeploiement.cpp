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
#include "MIL_PionMission_GEN_RealiserPlateFormeDeploiement.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_RealiserPlateFormeDeploiement::nDIAPositionObstacleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserPlateFormeDeploiement::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_RealiserPlateFormeDeploiement::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionObstacleIdx_ = DEC_Tools::InitializeDIAField( "positionObstacle_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserPlateFormeDeploiement constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserPlateFormeDeploiement::MIL_PionMission_GEN_RealiserPlateFormeDeploiement( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserPlateFormeDeploiement destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserPlateFormeDeploiement::~MIL_PionMission_GEN_RealiserPlateFormeDeploiement()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserPlateFormeDeploiement::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_RealiserPlateFormeDeploiement::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_plate_forme_deploiement;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_obstacle, GetVariable( nDIAPositionObstacleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserPlateFormeDeploiement::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserPlateFormeDeploiement::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserPlateFormeDeploiement::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserPlateFormeDeploiement::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_RealiserPlateFormeDeploiement& mission = static_cast< MIL_PionMission_GEN_RealiserPlateFormeDeploiement& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionObstacleIdx_ ), GetVariable( nDIAPositionObstacleIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserPlateFormeDeploiement::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserPlateFormeDeploiement::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserPlateFormeDeploiement::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserPlateFormeDeploiement::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_realiser_plate_forme_deploiement;
    asnMsg.mission.u.mission_pion_gen_realiser_plate_forme_deploiement  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionObstacleIdx_ ), asnMission.position_obstacle );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserPlateFormeDeploiement::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserPlateFormeDeploiement::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_realiser_plate_forme_deploiement );
    ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_plate_forme_deploiement;

    NET_ASN_Tools::Delete( asnMission.position_obstacle );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
