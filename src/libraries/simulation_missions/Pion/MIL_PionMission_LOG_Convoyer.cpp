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

#include "Missions_pch.h"
#include "MIL_PionMission_LOG_Convoyer.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"



//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_Convoyer::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_Convoyer::InitializeDIA( const MIL_PionMissionType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetDIATypeName() );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_Convoyer constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_Convoyer::MIL_PionMission_LOG_Convoyer( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_Convoyer destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_Convoyer::~MIL_PionMission_LOG_Convoyer()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_Convoyer::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_Convoyer::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        


    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_Convoyer::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_Convoyer::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;


    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_Convoyer::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_Convoyer::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_Convoyer& mission = static_cast< MIL_PionMission_LOG_Convoyer& >( missionTmp );


    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_Convoyer::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_Convoyer::Terminate()
{

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_Convoyer::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_Convoyer::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_Convoyer& asnMission = *new ASN1T_Mission_Pion_LOG_Convoyer();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_convoyer;
    asnMsg.mission.u.mission_pion_log_convoyer  = &asnMission;


}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_Convoyer::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_Convoyer::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_convoyer );
    ASN1T_Mission_Pion_LOG_Convoyer& asnMission = *asnMsg.mission.u.mission_pion_log_convoyer;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
