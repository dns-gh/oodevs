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
#include "MIL_PionMission_INF_RepondreAAggression.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_INF_RepondreAAggression::nDIAAggresseurIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_RepondreAAggression::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_RepondreAAggression::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAAggresseurIdx_ = DEC_Tools::InitializeDIAField( "aggresseur_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_RepondreAAggression constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_RepondreAAggression::MIL_PionMission_INF_RepondreAAggression( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_RepondreAAggression destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_RepondreAAggression::~MIL_PionMission_INF_RepondreAAggression()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_RepondreAAggression::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_RepondreAAggression::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_RepondreAAggression& asnMission = *asnMsg.mission.u.mission_pion_inf_repondre_a_aggression;
    if( !NET_ASN_Tools::CopyAgentKnowledge( asnMission.aggresseur, GetVariable( nDIAAggresseurIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_RepondreAAggression::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_RepondreAAggression::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAgentKnowledge( GetVariable( nDIAAggresseurIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_RepondreAAggression::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_RepondreAAggression::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_RepondreAAggression& mission = static_cast< MIL_PionMission_INF_RepondreAAggression& >( missionTmp );

    NET_ASN_Tools::CopyAgentKnowledge( mission.GetVariable( nDIAAggresseurIdx_ ), GetVariable( nDIAAggresseurIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_RepondreAAggression::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_RepondreAAggression::Terminate()
{
    NET_ASN_Tools::ResetAgentKnowledge( GetVariable( nDIAAggresseurIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_RepondreAAggression::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_RepondreAAggression::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_RepondreAAggression& asnMission = *new ASN1T_Mission_Pion_INF_RepondreAAggression();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_repondre_a_aggression;
    asnMsg.mission.u.mission_pion_inf_repondre_a_aggression  = &asnMission;

    NET_ASN_Tools::CopyAgentKnowledge( GetVariable( nDIAAggresseurIdx_ ), asnMission.aggresseur, pion_.GetKnowledgeGroup().GetKSQuerier() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_RepondreAAggression::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_RepondreAAggression::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_repondre_a_aggression );
    ASN1T_Mission_Pion_INF_RepondreAAggression& asnMission = *asnMsg.mission.u.mission_pion_inf_repondre_a_aggression;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
