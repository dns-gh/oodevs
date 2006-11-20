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
#include "MIL_PionMission_Test_Heliporter.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_Test_Heliporter::nDIAAgentsIdx_ = 0 ;
int MIL_PionMission_Test_Heliporter::nDIAPointEmbarquementIdx_ = 0 ;
int MIL_PionMission_Test_Heliporter::nDIAPointDebarquementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Heliporter::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Test_Heliporter::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAAgentsIdx_ = DEC_Tools::InitializeDIAField( "agents_", diaType );
    nDIAPointEmbarquementIdx_ = DEC_Tools::InitializeDIAField( "pointEmbarquement_", diaType );
    nDIAPointDebarquementIdx_ = DEC_Tools::InitializeDIAField( "pointDebarquement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Heliporter constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Test_Heliporter::MIL_PionMission_Test_Heliporter( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Heliporter destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Test_Heliporter::~MIL_PionMission_Test_Heliporter()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Heliporter::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Test_Heliporter::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Test_Heliporter& asnMission = *asnMsg.mission.u.mission_pion_test_heliporter;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.agents, GetVariable( nDIAAgentsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_embarquement, GetVariable( nDIAPointEmbarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_debarquement, GetVariable( nDIAPointDebarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Heliporter::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_Test_Heliporter::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Heliporter::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_Test_Heliporter::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Test_Heliporter& mission = static_cast< MIL_PionMission_Test_Heliporter& >( missionTmp );

    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAAgentsIdx_ ), GetVariable( nDIAAgentsIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointEmbarquementIdx_ ), GetVariable( nDIAPointEmbarquementIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDebarquementIdx_ ), GetVariable( nDIAPointDebarquementIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Heliporter::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_Heliporter::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Test_Heliporter& asnMission = *new ASN1T_Mission_Pion_Test_Heliporter();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_test_heliporter;
    asnMsg.mission.u.mission_pion_test_heliporter  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAAgentsIdx_ ), asnMission.agents );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointEmbarquementIdx_ ), asnMission.point_embarquement );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDebarquementIdx_ ), asnMission.point_debarquement );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Heliporter::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_Heliporter::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_test_heliporter );
    ASN1T_Mission_Pion_Test_Heliporter& asnMission = *asnMsg.mission.u.mission_pion_test_heliporter;

    NET_ASN_Tools::Delete( asnMission.agents );
    NET_ASN_Tools::Delete( asnMission.point_embarquement );
    NET_ASN_Tools::Delete( asnMission.point_debarquement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
