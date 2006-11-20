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
#include "MIL_PionMission_Test_DestroyObject.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_Test_DestroyObject::nDIAObjetIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_DestroyObject::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Test_DestroyObject::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAObjetIdx_ = DEC_Tools::InitializeDIAField( "objet_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_DestroyObject constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Test_DestroyObject::MIL_PionMission_Test_DestroyObject( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_DestroyObject destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Test_DestroyObject::~MIL_PionMission_Test_DestroyObject()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_DestroyObject::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Test_DestroyObject::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Test_DestroyObject& asnMission = *asnMsg.mission.u.mission_pion_test_destroy_object;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.objet, GetVariable( nDIAObjetIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_DestroyObject::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_Test_DestroyObject::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Test_DestroyObject::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_Test_DestroyObject::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Test_DestroyObject& mission = static_cast< MIL_PionMission_Test_DestroyObject& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIAObjetIdx_ ), GetVariable( nDIAObjetIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_DestroyObject::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_DestroyObject::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Test_DestroyObject& asnMission = *new ASN1T_Mission_Pion_Test_DestroyObject();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_test_destroy_object;
    asnMsg.mission.u.mission_pion_test_destroy_object  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIAObjetIdx_ ), asnMission.objet, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_DestroyObject::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_DestroyObject::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_test_destroy_object );
    ASN1T_Mission_Pion_Test_DestroyObject& asnMission = *asnMsg.mission.u.mission_pion_test_destroy_object;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
