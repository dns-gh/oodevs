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
#include "MIL_PionMission_Test_Fire.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_Test_Fire::nDIAPointIdx_ = 0 ;
int MIL_PionMission_Test_Fire::nDIATirIndirectIdx_ = 0 ;
int MIL_PionMission_Test_Fire::nDIAMunitionsIdx_ = 0 ;
int MIL_PionMission_Test_Fire::nDIANbObusIdx_ = 0 ;
int MIL_PionMission_Test_Fire::nDIATargetsIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Fire::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Test_Fire::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointIdx_ = DEC_Tools::InitializeDIAField( "point_", diaType );
    nDIATirIndirectIdx_ = DEC_Tools::InitializeDIAField( "tirIndirect_", diaType );
    nDIAMunitionsIdx_ = DEC_Tools::InitializeDIAField( "munitions_", diaType );
    nDIANbObusIdx_ = DEC_Tools::InitializeDIAField( "nbObus_", diaType );
    nDIATargetsIdx_ = DEC_Tools::InitializeDIAField( "targets_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Fire constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_Test_Fire::MIL_PionMission_Test_Fire( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Fire destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_Test_Fire::~MIL_PionMission_Test_Fire()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Fire::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Test_Fire::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Test_Fire& asnMission = *asnMsg.mission.u.mission_pion_test_fire;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point, point_, GetVariable( nDIAPointIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.tir_indirect, GetVariable( nDIATirIndirectIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.munitions, GetVariable( nDIAMunitionsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNumeric( asnMission.nb_obus, GetVariable( nDIANbObusIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgentKnowledgeList( asnMission.targets, GetVariable( nDIATargetsIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Fire::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_Test_Fire::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( point_, GetVariable( nDIAPointIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIATirIndirectIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAMunitionsIdx_ ) );
    NET_ASN_Tools::ResetNumeric( GetVariable( nDIANbObusIdx_ ) );
    NET_ASN_Tools::ResetAgentKnowledgeList( GetVariable( nDIATargetsIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Fire::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_Test_Fire::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Test_Fire& mission = static_cast< MIL_PionMission_Test_Fire& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointIdx_ ), point_, GetVariable( nDIAPointIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIATirIndirectIdx_ ), GetVariable( nDIATirIndirectIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAMunitionsIdx_ ), GetVariable( nDIAMunitionsIdx_ ) );
    NET_ASN_Tools::CopyNumeric( mission.GetVariable( nDIANbObusIdx_ ), GetVariable( nDIANbObusIdx_ ) );
    NET_ASN_Tools::CopyAgentKnowledgeList( mission.GetVariable( nDIATargetsIdx_ ), GetVariable( nDIATargetsIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Fire::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_Fire::Terminate()
{
    NET_ASN_Tools::ResetAgentKnowledgeList( GetVariable( nDIATargetsIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Fire::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_Fire::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Test_Fire& asnMission = *new ASN1T_Mission_Pion_Test_Fire();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_test_fire;
    asnMsg.mission.u.mission_pion_test_fire  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointIdx_ ), asnMission.point );
    NET_ASN_Tools::CopyBool( GetVariable( nDIATirIndirectIdx_ ), asnMission.tir_indirect );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAMunitionsIdx_ ), asnMission.munitions );
    NET_ASN_Tools::CopyNumeric( GetVariable( nDIANbObusIdx_ ), asnMission.nb_obus );
    NET_ASN_Tools::CopyAgentKnowledgeList( GetVariable( nDIATargetsIdx_ ), asnMission.targets, pion_.GetKnowledgeGroup().GetKSQuerier() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Fire::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_Fire::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_test_fire );
    ASN1T_Mission_Pion_Test_Fire& asnMission = *asnMsg.mission.u.mission_pion_test_fire;

    NET_ASN_Tools::Delete( asnMission.point );
    NET_ASN_Tools::Delete( asnMission.targets );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
