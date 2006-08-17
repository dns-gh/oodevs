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
#include "MIL_PionMission_Decrocher.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_Decrocher::nDIAPointRegroupementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Decrocher::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Decrocher::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Decrocher constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Decrocher::MIL_PionMission_Decrocher( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Decrocher destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Decrocher::~MIL_PionMission_Decrocher()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Decrocher::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Decrocher::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Decrocher& asnMission = *asnMsg.mission.u.mission_pion_decrocher;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Decrocher::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_Decrocher::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Decrocher::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_Decrocher::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Decrocher& mission = static_cast< MIL_PionMission_Decrocher& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRegroupementIdx_ ), GetVariable( nDIAPointRegroupementIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Decrocher::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Decrocher::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Decrocher::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Decrocher::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Decrocher& asnMission = *new ASN1T_Mission_Pion_Decrocher();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_decrocher;
    asnMsg.mission.u.mission_pion_decrocher  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Decrocher::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Decrocher::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_decrocher );
    ASN1T_Mission_Pion_Decrocher& asnMission = *asnMsg.mission.u.mission_pion_decrocher;

    NET_ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
