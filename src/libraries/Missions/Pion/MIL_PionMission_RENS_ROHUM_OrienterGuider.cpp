// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:4 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_RENS_ROHUM_OrienterGuider.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_RENS_ROHUM_OrienterGuider::nDIAAmiIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider::InitializeDIA
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_RENS_ROHUM_OrienterGuider::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAAmiIdx_ = DEC_Tools::InitializeDIAField( "ami_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider constructor
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
MIL_PionMission_RENS_ROHUM_OrienterGuider::MIL_PionMission_RENS_ROHUM_OrienterGuider( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider destructor
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
MIL_PionMission_RENS_ROHUM_OrienterGuider::~MIL_PionMission_RENS_ROHUM_OrienterGuider()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider::Initialize
// Created: 2005-6-28 - 14:3:4
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
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
bool MIL_PionMission_RENS_ROHUM_OrienterGuider::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAgent( GetVariable( nDIAAmiIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider::Initialize
// Created: 2005-6-28 - 14:3:4
// -----------------------------------------------------------------------------
bool MIL_PionMission_RENS_ROHUM_OrienterGuider::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_RENS_ROHUM_OrienterGuider& mission = static_cast< MIL_PionMission_RENS_ROHUM_OrienterGuider& >( missionTmp );

    NET_ASN_Tools::CopyAgent( mission.GetVariable( nDIAAmiIdx_ ), GetVariable( nDIAAmiIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider::Terminate
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROHUM_OrienterGuider::Terminate()
{
    NET_ASN_Tools::ResetAgent( GetVariable( nDIAAmiIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROHUM_OrienterGuider::Serialize
// Created: 2005-6-28 - 14:3:4
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
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROHUM_OrienterGuider::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_rens_rohum_orienter_guider );
    ASN1T_Mission_Pion_RENS_ROHUM_OrienterGuider& asnMission = *asnMsg.mission.u.mission_pion_rens_rohum_orienter_guider;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
