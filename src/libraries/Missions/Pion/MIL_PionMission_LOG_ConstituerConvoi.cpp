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
#include "MIL_PionMission_LOG_ConstituerConvoi.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"



//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ConstituerConvoi::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_ConstituerConvoi::InitializeDIA( const MIL_PionMissionType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetDIATypeName() );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ConstituerConvoi constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_ConstituerConvoi::MIL_PionMission_LOG_ConstituerConvoi( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ConstituerConvoi destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_ConstituerConvoi::~MIL_PionMission_LOG_ConstituerConvoi()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ConstituerConvoi::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_ConstituerConvoi::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        


    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ConstituerConvoi::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_ConstituerConvoi::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;


    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ConstituerConvoi::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_ConstituerConvoi::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_ConstituerConvoi& mission = static_cast< MIL_PionMission_LOG_ConstituerConvoi& >( missionTmp );


    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ConstituerConvoi::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_ConstituerConvoi::Terminate()
{

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ConstituerConvoi::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_ConstituerConvoi::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_ConstituerConvoi& asnMission = *new ASN1T_Mission_Pion_LOG_ConstituerConvoi();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_constituer_convoi;
    asnMsg.mission.u.mission_pion_log_constituer_convoi  = &asnMission;


}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ConstituerConvoi::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_ConstituerConvoi::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_constituer_convoi );
    ASN1T_Mission_Pion_LOG_ConstituerConvoi& asnMission = *asnMsg.mission.u.mission_pion_log_constituer_convoi;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
