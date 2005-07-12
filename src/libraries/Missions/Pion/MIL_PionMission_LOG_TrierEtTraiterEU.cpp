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
#include "MIL_PionMission_LOG_TrierEtTraiterEU.h"

#include "Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "Entities/Orders/Pion/MIL_PionMissionType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"



//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterEU::InitializeDIA
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_TrierEtTraiterEU::InitializeDIA( const MIL_PionMissionType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetDIATypeName() );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterEU constructor
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_TrierEtTraiterEU::MIL_PionMission_LOG_TrierEtTraiterEU( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterEU destructor
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_TrierEtTraiterEU::~MIL_PionMission_LOG_TrierEtTraiterEU()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterEU::Initialize
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_TrierEtTraiterEU::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        


    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterEU::Initialize
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_TrierEtTraiterEU::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;


    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterEU::Initialize
// Created: 2005-5-9 - 11:49:5
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_TrierEtTraiterEU::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_TrierEtTraiterEU& mission = static_cast< MIL_PionMission_LOG_TrierEtTraiterEU& >( missionTmp );


    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterEU::Terminate
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TrierEtTraiterEU::Terminate()
{

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterEU::Serialize
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TrierEtTraiterEU::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_TrierEtTraiterEU& asnMission = *new ASN1T_Mission_Pion_LOG_TrierEtTraiterEU();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_trier_et_traiter_eu;
    asnMsg.mission.u.mission_pion_log_trier_et_traiter_eu  = &asnMission;


}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterEU::CleanAfterSerialization
// Created: 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TrierEtTraiterEU::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_trier_et_traiter_eu );
    ASN1T_Mission_Pion_LOG_TrierEtTraiterEU& asnMission = *asnMsg.mission.u.mission_pion_log_trier_et_traiter_eu;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
