// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:4 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_Test_CreateObject.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_Test_CreateObject::nDIAFormeIdx_ = 0 ;
int MIL_PionMission_Test_CreateObject::nDIATypeIdx_ = 0 ;
int MIL_PionMission_Test_CreateObject::nDIAParamIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_CreateObject::InitializeDIA
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Test_CreateObject::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAFormeIdx_ = DEC_Tools::InitializeDIAField( "forme_", diaType );
    nDIATypeIdx_ = DEC_Tools::InitializeDIAField( "type_", diaType );
    nDIAParamIdx_ = DEC_Tools::InitializeDIAField( "param_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_CreateObject constructor
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
MIL_PionMission_Test_CreateObject::MIL_PionMission_Test_CreateObject( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_CreateObject destructor
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
MIL_PionMission_Test_CreateObject::~MIL_PionMission_Test_CreateObject()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_CreateObject::Initialize
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Test_CreateObject::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Test_CreateObject& asnMission = *asnMsg.mission.u.mission_pion_test_create_object;
    if( !NET_ASN_Tools::CopyLocation( asnMission.forme, forme_, GetVariable( nDIAFormeIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.type, GetVariable( nDIATypeIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNumeric( asnMission.param, GetVariable( nDIAParamIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_CreateObject::Initialize
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
bool MIL_PionMission_Test_CreateObject::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetLocation( forme_, GetVariable( nDIAFormeIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIATypeIdx_ ) );
    NET_ASN_Tools::ResetNumeric( GetVariable( nDIAParamIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Test_CreateObject::Initialize
// Created: 2005-6-28 - 14:3:4
// -----------------------------------------------------------------------------
bool MIL_PionMission_Test_CreateObject::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Test_CreateObject& mission = static_cast< MIL_PionMission_Test_CreateObject& >( missionTmp );

    NET_ASN_Tools::CopyLocation( mission.GetVariable( nDIAFormeIdx_ ), forme_, GetVariable( nDIAFormeIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIATypeIdx_ ), GetVariable( nDIATypeIdx_ ) );
    NET_ASN_Tools::CopyNumeric( mission.GetVariable( nDIAParamIdx_ ), GetVariable( nDIAParamIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_CreateObject::Terminate
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_CreateObject::Terminate()
{
    NET_ASN_Tools::ResetNumeric( GetVariable( nDIAParamIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_CreateObject::Serialize
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_CreateObject::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Test_CreateObject& asnMission = *new ASN1T_Mission_Pion_Test_CreateObject();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_test_create_object;
    asnMsg.mission.u.mission_pion_test_create_object  = &asnMission;

    NET_ASN_Tools::CopyLocation( GetVariable( nDIAFormeIdx_ ), asnMission.forme );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIATypeIdx_ ), asnMission.type );
    NET_ASN_Tools::CopyNumeric( GetVariable( nDIAParamIdx_ ), asnMission.param );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_CreateObject::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_CreateObject::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_test_create_object );
    ASN1T_Mission_Pion_Test_CreateObject& asnMission = *asnMsg.mission.u.mission_pion_test_create_object;

    NET_ASN_Tools::Delete( asnMission.forme );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
