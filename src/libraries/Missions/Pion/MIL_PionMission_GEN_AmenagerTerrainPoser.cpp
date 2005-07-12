// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:3 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_GEN_AmenagerTerrainPoser.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_AmenagerTerrainPoser::nDIAPosObstacleIdx_ = 0 ;
int MIL_PionMission_GEN_AmenagerTerrainPoser::nDIAOidObstaclePlanifieIdx_ = 0 ;
int MIL_PionMission_GEN_AmenagerTerrainPoser::nDIATypeObjetIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AmenagerTerrainPoser::InitializeDIA
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_AmenagerTerrainPoser::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPosObstacleIdx_ = DEC_Tools::InitializeDIAField( "posObstacle_", diaType );
    nDIAOidObstaclePlanifieIdx_ = DEC_Tools::InitializeDIAField( "oidObstaclePlanifie_", diaType );
    nDIATypeObjetIdx_ = DEC_Tools::InitializeDIAField( "typeObjet_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AmenagerTerrainPoser constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_AmenagerTerrainPoser::MIL_PionMission_GEN_AmenagerTerrainPoser( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AmenagerTerrainPoser destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_AmenagerTerrainPoser::~MIL_PionMission_GEN_AmenagerTerrainPoser()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AmenagerTerrainPoser::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_AmenagerTerrainPoser::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_AmenagerTerrainPoser& asnMission = *asnMsg.mission.u.mission_pion_gen_amenager_terrain_poser;
    if( !NET_ASN_Tools::CopyPoint( asnMission.pos_obstacle, posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyID( asnMission.oid_obstacle_planifie, GetVariable( nDIAOidObstaclePlanifieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.type_objet, GetVariable( nDIATypeObjetIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AmenagerTerrainPoser::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_AmenagerTerrainPoser::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) );
    NET_ASN_Tools::ResetID( GetVariable( nDIAOidObstaclePlanifieIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIATypeObjetIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AmenagerTerrainPoser::Initialize
// Created: 2005-6-28 - 14:3:3
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_AmenagerTerrainPoser::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_AmenagerTerrainPoser& mission = static_cast< MIL_PionMission_GEN_AmenagerTerrainPoser& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPosObstacleIdx_ ), posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) );
    NET_ASN_Tools::CopyID( mission.GetVariable( nDIAOidObstaclePlanifieIdx_ ), GetVariable( nDIAOidObstaclePlanifieIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIATypeObjetIdx_ ), GetVariable( nDIATypeObjetIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AmenagerTerrainPoser::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_AmenagerTerrainPoser::Terminate()
{
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIATypeObjetIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AmenagerTerrainPoser::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_AmenagerTerrainPoser::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_AmenagerTerrainPoser& asnMission = *new ASN1T_Mission_Pion_GEN_AmenagerTerrainPoser();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_amenager_terrain_poser;
    asnMsg.mission.u.mission_pion_gen_amenager_terrain_poser  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPosObstacleIdx_ ), asnMission.pos_obstacle );
    NET_ASN_Tools::CopyID( GetVariable( nDIAOidObstaclePlanifieIdx_ ), asnMission.oid_obstacle_planifie );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIATypeObjetIdx_ ), asnMission.type_objet );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_AmenagerTerrainPoser::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_AmenagerTerrainPoser::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_amenager_terrain_poser );
    ASN1T_Mission_Pion_GEN_AmenagerTerrainPoser& asnMission = *asnMsg.mission.u.mission_pion_gen_amenager_terrain_poser;

    NET_ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
