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
#include "MIL_PionMission_GEN_RealiserAireLogistique.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_RealiserAireLogistique::nDIAPositionObstacleIdx_ = 0 ;
int MIL_PionMission_GEN_RealiserAireLogistique::nDIAOidObstaclePlanifieIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserAireLogistique::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_RealiserAireLogistique::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionObstacleIdx_ = DEC_Tools::InitializeDIAField( "positionObstacle_", diaType );
    nDIAOidObstaclePlanifieIdx_ = DEC_Tools::InitializeDIAField( "oidObstaclePlanifie_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserAireLogistique constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserAireLogistique::MIL_PionMission_GEN_RealiserAireLogistique( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserAireLogistique destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserAireLogistique::~MIL_PionMission_GEN_RealiserAireLogistique()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserAireLogistique::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_RealiserAireLogistique::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_RealiserAireLogistique& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_aire_logistique;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_obstacle, positionObstacle_, GetVariable( nDIAPositionObstacleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyID( asnMission.oid_obstacle_planifie, GetVariable( nDIAOidObstaclePlanifieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserAireLogistique::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserAireLogistique::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( positionObstacle_, GetVariable( nDIAPositionObstacleIdx_ ) );
    NET_ASN_Tools::ResetID( GetVariable( nDIAOidObstaclePlanifieIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserAireLogistique::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserAireLogistique::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_RealiserAireLogistique& mission = static_cast< MIL_PionMission_GEN_RealiserAireLogistique& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionObstacleIdx_ ), positionObstacle_, GetVariable( nDIAPositionObstacleIdx_ ) );
    NET_ASN_Tools::CopyID( mission.GetVariable( nDIAOidObstaclePlanifieIdx_ ), GetVariable( nDIAOidObstaclePlanifieIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserAireLogistique::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserAireLogistique::Terminate()
{
    NET_ASN_Tools::ResetID( GetVariable( nDIAOidObstaclePlanifieIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserAireLogistique::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserAireLogistique::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_RealiserAireLogistique& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserAireLogistique();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_realiser_aire_logistique;
    asnMsg.mission.u.mission_pion_gen_realiser_aire_logistique  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionObstacleIdx_ ), asnMission.position_obstacle );
    NET_ASN_Tools::CopyID( GetVariable( nDIAOidObstaclePlanifieIdx_ ), asnMission.oid_obstacle_planifie );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserAireLogistique::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserAireLogistique::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_realiser_aire_logistique );
    ASN1T_Mission_Pion_GEN_RealiserAireLogistique& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_aire_logistique;

    NET_ASN_Tools::Delete( asnMission.position_obstacle );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
