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
#include "MIL_PionMission_GEN_DetruireRoute.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_DetruireRoute::nDIAPosObstacleIdx_ = 0 ;
int MIL_PionMission_GEN_DetruireRoute::nDIAOidObstaclePlanifieIdx_ = 0 ;
int MIL_PionMission_GEN_DetruireRoute::nDIAPreliminaireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruireRoute::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_DetruireRoute::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPosObstacleIdx_ = DEC_Tools::InitializeDIAField( "posObstacle_", diaType );
    nDIAOidObstaclePlanifieIdx_ = DEC_Tools::InitializeDIAField( "oidObstaclePlanifie_", diaType );
    nDIAPreliminaireIdx_ = DEC_Tools::InitializeDIAField( "preliminaire_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruireRoute constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_DetruireRoute::MIL_PionMission_GEN_DetruireRoute( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruireRoute destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_DetruireRoute::~MIL_PionMission_GEN_DetruireRoute()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruireRoute::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_DetruireRoute::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_DetruireRoute& asnMission = *asnMsg.mission.u.mission_pion_gen_detruire_route;
    if( !NET_ASN_Tools::CopyPoint( asnMission.pos_obstacle, posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyID( asnMission.oid_obstacle_planifie, GetVariable( nDIAOidObstaclePlanifieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.preliminaire, GetVariable( nDIAPreliminaireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruireRoute::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_DetruireRoute::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) );
    NET_ASN_Tools::ResetID( GetVariable( nDIAOidObstaclePlanifieIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAPreliminaireIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruireRoute::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_DetruireRoute::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_DetruireRoute& mission = static_cast< MIL_PionMission_GEN_DetruireRoute& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPosObstacleIdx_ ), posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) );
    NET_ASN_Tools::CopyID( mission.GetVariable( nDIAOidObstaclePlanifieIdx_ ), GetVariable( nDIAOidObstaclePlanifieIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAPreliminaireIdx_ ), GetVariable( nDIAPreliminaireIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruireRoute::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DetruireRoute::Terminate()
{
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAPreliminaireIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruireRoute::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DetruireRoute::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_DetruireRoute& asnMission = *new ASN1T_Mission_Pion_GEN_DetruireRoute();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_detruire_route;
    asnMsg.mission.u.mission_pion_gen_detruire_route  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPosObstacleIdx_ ), asnMission.pos_obstacle );
    NET_ASN_Tools::CopyID( GetVariable( nDIAOidObstaclePlanifieIdx_ ), asnMission.oid_obstacle_planifie );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPreliminaireIdx_ ), asnMission.preliminaire );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruireRoute::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DetruireRoute::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_detruire_route );
    ASN1T_Mission_Pion_GEN_DetruireRoute& asnMission = *asnMsg.mission.u.mission_pion_gen_detruire_route;

    NET_ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
