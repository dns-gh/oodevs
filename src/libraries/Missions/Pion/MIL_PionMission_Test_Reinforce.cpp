// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_Test_Reinforce.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_Test_Reinforce::nDIAUniteIdx_ = 0 ;
int MIL_PionMission_Test_Reinforce::nDIARenforceIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Reinforce::InitializeDIA
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Test_Reinforce::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUniteIdx_ = DEC_Tools::InitializeDIAField( "unite_", diaType );
    nDIARenforceIdx_ = DEC_Tools::InitializeDIAField( "renforce_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Reinforce constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_Test_Reinforce::MIL_PionMission_Test_Reinforce( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Reinforce destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_Test_Reinforce::~MIL_PionMission_Test_Reinforce()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Reinforce::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Test_Reinforce::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Test_Reinforce& asnMission = *asnMsg.mission.u.mission_pion_test_reinforce;
    if( !NET_ASN_Tools::CopyAgent( asnMission.unite, GetVariable( nDIAUniteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.renforce, GetVariable( nDIARenforceIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Reinforce::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
bool MIL_PionMission_Test_Reinforce::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAgent( GetVariable( nDIAUniteIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIARenforceIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Reinforce::Initialize
// Created: 2005-08-01 - 11:23:53
// -----------------------------------------------------------------------------
bool MIL_PionMission_Test_Reinforce::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Test_Reinforce& mission = static_cast< MIL_PionMission_Test_Reinforce& >( missionTmp );

    NET_ASN_Tools::CopyAgent( mission.GetVariable( nDIAUniteIdx_ ), GetVariable( nDIAUniteIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIARenforceIdx_ ), GetVariable( nDIARenforceIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Reinforce::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_Reinforce::Terminate()
{
    NET_ASN_Tools::ResetBool( GetVariable( nDIARenforceIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Reinforce::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_Reinforce::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Test_Reinforce& asnMission = *new ASN1T_Mission_Pion_Test_Reinforce();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_test_reinforce;
    asnMsg.mission.u.mission_pion_test_reinforce  = &asnMission;

    NET_ASN_Tools::CopyAgent( GetVariable( nDIAUniteIdx_ ), asnMission.unite );
    NET_ASN_Tools::CopyBool( GetVariable( nDIARenforceIdx_ ), asnMission.renforce );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_Reinforce::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_Reinforce::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_test_reinforce );
    ASN1T_Mission_Pion_Test_Reinforce& asnMission = *asnMsg.mission.u.mission_pion_test_reinforce;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
