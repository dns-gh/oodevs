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
#include "MIL_PionMission_GEN_DetruirePont.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_DetruirePont::nDIAPosObstacleIdx_ = 0 ;
int MIL_PionMission_GEN_DetruirePont::nDIAOidObstaclePlanifieIdx_ = 0 ;
int MIL_PionMission_GEN_DetruirePont::nDIAPreliminaireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruirePont::InitializeDIA
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_DetruirePont::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPosObstacleIdx_ = DEC_Tools::InitializeDIAField( "posObstacle_", diaType );
    nDIAOidObstaclePlanifieIdx_ = DEC_Tools::InitializeDIAField( "oidObstaclePlanifie_", diaType );
    nDIAPreliminaireIdx_ = DEC_Tools::InitializeDIAField( "preliminaire_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruirePont constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_DetruirePont::MIL_PionMission_GEN_DetruirePont( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruirePont destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_DetruirePont::~MIL_PionMission_GEN_DetruirePont()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruirePont::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_DetruirePont::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_DetruirePont& asnMission = *asnMsg.mission.u.mission_pion_gen_detruire_pont;
    if( !NET_ASN_Tools::CopyPoint( asnMission.pos_obstacle, posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyID( asnMission.oid_obstacle_planifie, GetVariable( nDIAOidObstaclePlanifieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.preliminaire, GetVariable( nDIAPreliminaireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruirePont::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_DetruirePont::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) );
    NET_ASN_Tools::ResetID( GetVariable( nDIAOidObstaclePlanifieIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAPreliminaireIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruirePont::Initialize
// Created: 2005-6-28 - 14:3:3
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_DetruirePont::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_DetruirePont& mission = static_cast< MIL_PionMission_GEN_DetruirePont& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPosObstacleIdx_ ), posObstacle_, GetVariable( nDIAPosObstacleIdx_ ) );
    NET_ASN_Tools::CopyID( mission.GetVariable( nDIAOidObstaclePlanifieIdx_ ), GetVariable( nDIAOidObstaclePlanifieIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAPreliminaireIdx_ ), GetVariable( nDIAPreliminaireIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruirePont::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DetruirePont::Terminate()
{
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAPreliminaireIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruirePont::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DetruirePont::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_DetruirePont& asnMission = *new ASN1T_Mission_Pion_GEN_DetruirePont();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_detruire_pont;
    asnMsg.mission.u.mission_pion_gen_detruire_pont  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPosObstacleIdx_ ), asnMission.pos_obstacle );
    NET_ASN_Tools::CopyID( GetVariable( nDIAOidObstaclePlanifieIdx_ ), asnMission.oid_obstacle_planifie );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPreliminaireIdx_ ), asnMission.preliminaire );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DetruirePont::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DetruirePont::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_detruire_pont );
    ASN1T_Mission_Pion_GEN_DetruirePont& asnMission = *asnMsg.mission.u.mission_pion_gen_detruire_pont;

    NET_ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
