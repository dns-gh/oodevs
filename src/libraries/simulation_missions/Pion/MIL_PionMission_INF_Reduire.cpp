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
#include "MIL_PionMission_INF_Reduire.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_INF_Reduire::nDIAUniteAReduireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Reduire::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_Reduire::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUniteAReduireIdx_ = DEC_Tools::InitializeDIAField( "uniteAReduire_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Reduire constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Reduire::MIL_PionMission_INF_Reduire( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Reduire destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Reduire::~MIL_PionMission_INF_Reduire()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Reduire::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_Reduire::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_Reduire& asnMission = *asnMsg.mission.u.mission_pion_inf_reduire;
    if( !NET_ASN_Tools::CopyAgentKnowledge( asnMission.unite_a_reduire, GetVariable( nDIAUniteAReduireIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Reduire::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_Reduire::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Reduire::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_Reduire::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_Reduire& mission = static_cast< MIL_PionMission_INF_Reduire& >( missionTmp );

    NET_ASN_Tools::CopyAgentKnowledge( mission.GetVariable( nDIAUniteAReduireIdx_ ), GetVariable( nDIAUniteAReduireIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Reduire::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Reduire::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_Reduire& asnMission = *new ASN1T_Mission_Pion_INF_Reduire();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_reduire;
    asnMsg.mission.u.mission_pion_inf_reduire  = &asnMission;

    NET_ASN_Tools::CopyAgentKnowledge( GetVariable( nDIAUniteAReduireIdx_ ), asnMission.unite_a_reduire, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Reduire::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Reduire::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_reduire );
    ASN1T_Mission_Pion_INF_Reduire& asnMission = *asnMsg.mission.u.mission_pion_inf_reduire;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
