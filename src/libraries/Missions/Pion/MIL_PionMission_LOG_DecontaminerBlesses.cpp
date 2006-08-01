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
#include "MIL_PionMission_LOG_DecontaminerBlesses.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_LOG_DecontaminerBlesses::nDIAPositionDeploiementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_DecontaminerBlesses::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_DecontaminerBlesses::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionDeploiementIdx_ = DEC_Tools::InitializeDIAField( "positionDeploiement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_DecontaminerBlesses constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_DecontaminerBlesses::MIL_PionMission_LOG_DecontaminerBlesses( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_DecontaminerBlesses destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_DecontaminerBlesses::~MIL_PionMission_LOG_DecontaminerBlesses()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_DecontaminerBlesses::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_DecontaminerBlesses::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_LOG_DecontaminerBlesses& asnMission = *asnMsg.mission.u.mission_pion_log_decontaminer_blesses;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_deploiement, GetVariable( nDIAPositionDeploiementIdx_ ), asnMission.m.position_deploiementPresent ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_DecontaminerBlesses::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_DecontaminerBlesses::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_DecontaminerBlesses::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_DecontaminerBlesses::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_DecontaminerBlesses& mission = static_cast< MIL_PionMission_LOG_DecontaminerBlesses& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionDeploiementIdx_ ), GetVariable( nDIAPositionDeploiementIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_DecontaminerBlesses::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_DecontaminerBlesses::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_DecontaminerBlesses::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_DecontaminerBlesses::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_DecontaminerBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_DecontaminerBlesses();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_decontaminer_blesses;
    asnMsg.mission.u.mission_pion_log_decontaminer_blesses  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionDeploiementIdx_ ), asnMission.position_deploiement );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_DecontaminerBlesses::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_DecontaminerBlesses::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_decontaminer_blesses );
    ASN1T_Mission_Pion_LOG_DecontaminerBlesses& asnMission = *asnMsg.mission.u.mission_pion_log_decontaminer_blesses;

    NET_ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
