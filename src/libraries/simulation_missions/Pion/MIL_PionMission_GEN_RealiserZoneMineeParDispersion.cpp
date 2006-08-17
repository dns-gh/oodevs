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
#include "MIL_PionMission_GEN_RealiserZoneMineeParDispersion.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_RealiserZoneMineeParDispersion::nDIAPosObstacleIdx_ = 0 ;
int MIL_PionMission_GEN_RealiserZoneMineeParDispersion::nDIAOidObstaclePlanifieIdx_ = 0 ;
int MIL_PionMission_GEN_RealiserZoneMineeParDispersion::nDIADensiteIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserZoneMineeParDispersion::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_RealiserZoneMineeParDispersion::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPosObstacleIdx_ = DEC_Tools::InitializeDIAField( "posObstacle_", diaType );
    nDIAOidObstaclePlanifieIdx_ = DEC_Tools::InitializeDIAField( "oidObstaclePlanifie_", diaType );
    nDIADensiteIdx_ = DEC_Tools::InitializeDIAField( "densite_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserZoneMineeParDispersion constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserZoneMineeParDispersion::MIL_PionMission_GEN_RealiserZoneMineeParDispersion( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserZoneMineeParDispersion destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserZoneMineeParDispersion::~MIL_PionMission_GEN_RealiserZoneMineeParDispersion()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserZoneMineeParDispersion::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_RealiserZoneMineeParDispersion::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_zone_minee_par_dispersion;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.pos_obstacle, GetVariable( nDIAPosObstacleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyID( asnMission.oid_obstacle_planifie, GetVariable( nDIAOidObstaclePlanifieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNumeric( asnMission.densite, GetVariable( nDIADensiteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserZoneMineeParDispersion::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserZoneMineeParDispersion::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

            
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserZoneMineeParDispersion::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserZoneMineeParDispersion::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_RealiserZoneMineeParDispersion& mission = static_cast< MIL_PionMission_GEN_RealiserZoneMineeParDispersion& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIAPosObstacleIdx_ ), GetVariable( nDIAPosObstacleIdx_ ) );
    NET_ASN_Tools::CopyID( mission.GetVariable( nDIAOidObstaclePlanifieIdx_ ), GetVariable( nDIAOidObstaclePlanifieIdx_ ) );
    NET_ASN_Tools::CopyNumeric( mission.GetVariable( nDIADensiteIdx_ ), GetVariable( nDIADensiteIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserZoneMineeParDispersion::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserZoneMineeParDispersion::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserZoneMineeParDispersion::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserZoneMineeParDispersion::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_realiser_zone_minee_par_dispersion;
    asnMsg.mission.u.mission_pion_gen_realiser_zone_minee_par_dispersion  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAPosObstacleIdx_ ), asnMission.pos_obstacle );
    NET_ASN_Tools::CopyID( GetVariable( nDIAOidObstaclePlanifieIdx_ ), asnMission.oid_obstacle_planifie );
    NET_ASN_Tools::CopyNumeric( GetVariable( nDIADensiteIdx_ ), asnMission.densite );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserZoneMineeParDispersion::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserZoneMineeParDispersion::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_realiser_zone_minee_par_dispersion );
    ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_zone_minee_par_dispersion;

    NET_ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
