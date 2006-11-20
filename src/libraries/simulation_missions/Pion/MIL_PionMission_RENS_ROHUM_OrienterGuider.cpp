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
#include "MIL_PionMission_RENS_ROHUM_OrienterGuider.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_RENS_ROHUM_OrienterGuider::nDIAAmiIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_RENS_ROHUM_OrienterGuider::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAAmiIdx_ = DEC_Tools::InitializeDIAField( "ami_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_RENS_ROHUM_OrienterGuider::MIL_PionMission_RENS_ROHUM_OrienterGuider( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_RENS_ROHUM_OrienterGuider::~MIL_PionMission_RENS_ROHUM_OrienterGuider()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_RENS_ROHUM_OrienterGuider::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_RENS_ROHUM_OrienterGuider& asnMission = *asnMsg.mission.u.mission_pion_rens_rohum_orienter_guider;
    if( !NET_ASN_Tools::CopyAgent( asnMission.ami, GetVariable( nDIAAmiIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_RENS_ROHUM_OrienterGuider::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_RENS_ROHUM_OrienterGuider::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_RENS_ROHUM_OrienterGuider& mission = static_cast< MIL_PionMission_RENS_ROHUM_OrienterGuider& >( missionTmp );

    NET_ASN_Tools::CopyAgent( mission.GetVariable( nDIAAmiIdx_ ), GetVariable( nDIAAmiIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROHUM_OrienterGuider::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_RENS_ROHUM_OrienterGuider& asnMission = *new ASN1T_Mission_Pion_RENS_ROHUM_OrienterGuider();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_rens_rohum_orienter_guider;
    asnMsg.mission.u.mission_pion_rens_rohum_orienter_guider  = &asnMission;

    NET_ASN_Tools::CopyAgent( GetVariable( nDIAAmiIdx_ ), asnMission.ami );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROHUM_OrienterGuider::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_rens_rohum_orienter_guider );
    ASN1T_Mission_Pion_RENS_ROHUM_OrienterGuider& asnMission = *asnMsg.mission.u.mission_pion_rens_rohum_orienter_guider;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
