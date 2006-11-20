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
#include "MIL_PionMission_INF_AppuyerSurPosition.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_INF_AppuyerSurPosition::nDIAPositionIdx_ = 0 ;
int MIL_PionMission_INF_AppuyerSurPosition::nDIAUnitesAAppuyerIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_AppuyerSurPosition::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_AppuyerSurPosition::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionIdx_ = DEC_Tools::InitializeDIAField( "position_", diaType );
    nDIAUnitesAAppuyerIdx_ = DEC_Tools::InitializeDIAField( "unitesAAppuyer_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_AppuyerSurPosition constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_AppuyerSurPosition::MIL_PionMission_INF_AppuyerSurPosition( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_AppuyerSurPosition destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_AppuyerSurPosition::~MIL_PionMission_INF_AppuyerSurPosition()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_AppuyerSurPosition::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_AppuyerSurPosition::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_AppuyerSurPosition& asnMission = *asnMsg.mission.u.mission_pion_inf_appuyer_sur_position;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position, GetVariable( nDIAPositionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_appuyer, GetVariable( nDIAUnitesAAppuyerIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_AppuyerSurPosition::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_AppuyerSurPosition::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_AppuyerSurPosition::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_AppuyerSurPosition::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_AppuyerSurPosition& mission = static_cast< MIL_PionMission_INF_AppuyerSurPosition& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionIdx_ ), GetVariable( nDIAPositionIdx_ ) );
    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAUnitesAAppuyerIdx_ ), GetVariable( nDIAUnitesAAppuyerIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_AppuyerSurPosition::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_AppuyerSurPosition::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_AppuyerSurPosition& asnMission = *new ASN1T_Mission_Pion_INF_AppuyerSurPosition();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_appuyer_sur_position;
    asnMsg.mission.u.mission_pion_inf_appuyer_sur_position  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionIdx_ ), asnMission.position );
    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesAAppuyerIdx_ ), asnMission.unites_a_appuyer );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_AppuyerSurPosition::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_AppuyerSurPosition::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_appuyer_sur_position );
    ASN1T_Mission_Pion_INF_AppuyerSurPosition& asnMission = *asnMsg.mission.u.mission_pion_inf_appuyer_sur_position;

    NET_ASN_Tools::Delete( asnMission.position );
    NET_ASN_Tools::Delete( asnMission.unites_a_appuyer );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
