// *****************************************************************************
//
// $Created: 2005-5-9 - 11:49:5 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_LOG_TraiterEU.h"

#include "Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "Entities/Orders/Pion/MIL_PionMissionType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"



//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TraiterEU::InitializeDIA
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_TraiterEU::InitializeDIA( const MIL_PionMissionType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetDIATypeName() );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TraiterEU constructor
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_TraiterEU::MIL_PionMission_LOG_TraiterEU( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TraiterEU destructor
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_TraiterEU::~MIL_PionMission_LOG_TraiterEU()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TraiterEU::Initialize
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_TraiterEU::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        


    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TraiterEU::Initialize
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_TraiterEU::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;


    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TraiterEU::Initialize
// Created: 2005-5-9 - 11:49:5
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_TraiterEU::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_TraiterEU& mission = static_cast< MIL_PionMission_LOG_TraiterEU& >( missionTmp );


    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TraiterEU::Terminate
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TraiterEU::Terminate()
{

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TraiterEU::Serialize
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TraiterEU::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_TraiterEU& asnMission = *new ASN1T_Mission_Pion_LOG_TraiterEU();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_traiter_eu;
    asnMsg.mission.u.mission_pion_log_traiter_eu  = &asnMission;


}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TraiterEU::CleanAfterSerialization
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TraiterEU::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_traiter_eu );
    ASN1T_Mission_Pion_LOG_TraiterEU& asnMission = *asnMsg.mission.u.mission_pion_log_traiter_eu;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
