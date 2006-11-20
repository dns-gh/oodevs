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
#include "MIL_PionMission_INF_ContreAttaquer.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_INF_ContreAttaquer::nDIAEnnemiIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ContreAttaquer::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_ContreAttaquer::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAEnnemiIdx_ = DEC_Tools::InitializeDIAField( "ennemi_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ContreAttaquer constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_ContreAttaquer::MIL_PionMission_INF_ContreAttaquer( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ContreAttaquer destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_ContreAttaquer::~MIL_PionMission_INF_ContreAttaquer()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ContreAttaquer::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_ContreAttaquer::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_ContreAttaquer& asnMission = *asnMsg.mission.u.mission_pion_inf_contre_attaquer;
    if( !NET_ASN_Tools::CopyAgentKnowledge( asnMission.ennemi, GetVariable( nDIAEnnemiIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ContreAttaquer::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_ContreAttaquer::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ContreAttaquer::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_ContreAttaquer::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_ContreAttaquer& mission = static_cast< MIL_PionMission_INF_ContreAttaquer& >( missionTmp );

    NET_ASN_Tools::CopyAgentKnowledge( mission.GetVariable( nDIAEnnemiIdx_ ), GetVariable( nDIAEnnemiIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ContreAttaquer::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_ContreAttaquer::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_ContreAttaquer& asnMission = *new ASN1T_Mission_Pion_INF_ContreAttaquer();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_contre_attaquer;
    asnMsg.mission.u.mission_pion_inf_contre_attaquer  = &asnMission;

    NET_ASN_Tools::CopyAgentKnowledge( GetVariable( nDIAEnnemiIdx_ ), asnMission.ennemi, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ContreAttaquer::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_ContreAttaquer::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_contre_attaquer );
    ASN1T_Mission_Pion_INF_ContreAttaquer& asnMission = *asnMsg.mission.u.mission_pion_inf_contre_attaquer;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
