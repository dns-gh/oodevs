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
#include "MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::nDIAPontFlottantIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPontFlottantIdx_ = DEC_Tools::InitializeDIAField( "pontFlottant_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::~MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint& asnMission = *asnMsg.mission.u.mission_pion_log_appuyer_franchissement_sur_un_point;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.pont_flottant, GetVariable( nDIAPontFlottantIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint& mission = static_cast< MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIAPontFlottantIdx_ ), GetVariable( nDIAPontFlottantIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint& asnMission = *new ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_appuyer_franchissement_sur_un_point;
    asnMsg.mission.u.mission_pion_log_appuyer_franchissement_sur_un_point  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIAPontFlottantIdx_ ), asnMission.pont_flottant, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_appuyer_franchissement_sur_un_point );
    ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint& asnMission = *asnMsg.mission.u.mission_pion_log_appuyer_franchissement_sur_un_point;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
