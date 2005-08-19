// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_GEN_RealiserFosseAC.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_RealiserFosseAC::nDIAPosObstacleIdx_ = 0 ;
int MIL_PionMission_GEN_RealiserFosseAC::nDIAOidObstaclePlanifieIdx_ = 0 ;
int MIL_PionMission_GEN_RealiserFosseAC::nDIATypeIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserFosseAC::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_RealiserFosseAC::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPosObstacleIdx_ = DEC_Tools::InitializeDIAField( "posObstacle_", diaType );
    nDIAOidObstaclePlanifieIdx_ = DEC_Tools::InitializeDIAField( "oidObstaclePlanifie_", diaType );
    nDIATypeIdx_ = DEC_Tools::InitializeDIAField( "type_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserFosseAC constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserFosseAC::MIL_PionMission_GEN_RealiserFosseAC( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserFosseAC destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserFosseAC::~MIL_PionMission_GEN_RealiserFosseAC()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserFosseAC::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_RealiserFosseAC::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_RealiserFosseAC& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_fosse_ac;
    if( !NET_ASN_Tools::CopyLocation( asnMission.pos_obstacle, posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyID( asnMission.oid_obstacle_planifie, GetVariable( nDIAOidObstaclePlanifieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.type, GetVariable( nDIATypeIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserFosseAC::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserFosseAC::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetLocation( posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) );
    NET_ASN_Tools::ResetID( GetVariable( nDIAOidObstaclePlanifieIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIATypeIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserFosseAC::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserFosseAC::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_RealiserFosseAC& mission = static_cast< MIL_PionMission_GEN_RealiserFosseAC& >( missionTmp );

    NET_ASN_Tools::CopyLocation( mission.GetVariable( nDIAPosObstacleIdx_ ), posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) );
    NET_ASN_Tools::CopyID( mission.GetVariable( nDIAOidObstaclePlanifieIdx_ ), GetVariable( nDIAOidObstaclePlanifieIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIATypeIdx_ ), GetVariable( nDIATypeIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserFosseAC::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserFosseAC::Terminate()
{
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIATypeIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserFosseAC::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserFosseAC::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_RealiserFosseAC& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserFosseAC();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_realiser_fosse_ac;
    asnMsg.mission.u.mission_pion_gen_realiser_fosse_ac  = &asnMission;

    NET_ASN_Tools::CopyLocation( GetVariable( nDIAPosObstacleIdx_ ), asnMission.pos_obstacle );
    NET_ASN_Tools::CopyID( GetVariable( nDIAOidObstaclePlanifieIdx_ ), asnMission.oid_obstacle_planifie );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIATypeIdx_ ), asnMission.type );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserFosseAC::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserFosseAC::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_realiser_fosse_ac );
    ASN1T_Mission_Pion_GEN_RealiserFosseAC& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_fosse_ac;

    NET_ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
