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
#include "MIL_PionMission_Test_PathFind.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_Test_PathFind::nDIANbPathfindIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_PathFind::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Test_PathFind::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIANbPathfindIdx_ = DEC_Tools::InitializeDIAField( "nbPathfind_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_PathFind constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_Test_PathFind::MIL_PionMission_Test_PathFind( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_PathFind destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_Test_PathFind::~MIL_PionMission_Test_PathFind()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_PathFind::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Test_PathFind::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Test_PathFind& asnMission = *asnMsg.mission.u.mission_pion_test_path_find;
    if( !NET_ASN_Tools::CopyNumeric( asnMission.nb_pathfind, GetVariable( nDIANbPathfindIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_PathFind::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_Test_PathFind::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetNumeric( GetVariable( nDIANbPathfindIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Test_PathFind::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_Test_PathFind::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Test_PathFind& mission = static_cast< MIL_PionMission_Test_PathFind& >( missionTmp );

    NET_ASN_Tools::CopyNumeric( mission.GetVariable( nDIANbPathfindIdx_ ), GetVariable( nDIANbPathfindIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_PathFind::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_PathFind::Terminate()
{
    NET_ASN_Tools::ResetNumeric( GetVariable( nDIANbPathfindIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_PathFind::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_PathFind::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Test_PathFind& asnMission = *new ASN1T_Mission_Pion_Test_PathFind();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_test_path_find;
    asnMsg.mission.u.mission_pion_test_path_find  = &asnMission;

    NET_ASN_Tools::CopyNumeric( GetVariable( nDIANbPathfindIdx_ ), asnMission.nb_pathfind );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_PathFind::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_PathFind::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_test_path_find );
    ASN1T_Mission_Pion_Test_PathFind& asnMission = *asnMsg.mission.u.mission_pion_test_path_find;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
