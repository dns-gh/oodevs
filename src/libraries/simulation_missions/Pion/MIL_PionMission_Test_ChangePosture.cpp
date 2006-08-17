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
#include "MIL_PionMission_Test_ChangePosture.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_Test_ChangePosture::nDIAPostureIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_ChangePosture::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Test_ChangePosture::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPostureIdx_ = DEC_Tools::InitializeDIAField( "posture_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_ChangePosture constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Test_ChangePosture::MIL_PionMission_Test_ChangePosture( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_ChangePosture destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Test_ChangePosture::~MIL_PionMission_Test_ChangePosture()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_ChangePosture::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Test_ChangePosture::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Test_ChangePosture& asnMission = *asnMsg.mission.u.mission_pion_test_change_posture;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.posture, GetVariable( nDIAPostureIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_ChangePosture::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_Test_ChangePosture::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Test_ChangePosture::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_Test_ChangePosture::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Test_ChangePosture& mission = static_cast< MIL_PionMission_Test_ChangePosture& >( missionTmp );

    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAPostureIdx_ ), GetVariable( nDIAPostureIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_ChangePosture::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_ChangePosture::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_ChangePosture::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_ChangePosture::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Test_ChangePosture& asnMission = *new ASN1T_Mission_Pion_Test_ChangePosture();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_test_change_posture;
    asnMsg.mission.u.mission_pion_test_change_posture  = &asnMission;

    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPostureIdx_ ), asnMission.posture );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_ChangePosture::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_ChangePosture::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_test_change_posture );
    ASN1T_Mission_Pion_Test_ChangePosture& asnMission = *asnMsg.mission.u.mission_pion_test_change_posture;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
