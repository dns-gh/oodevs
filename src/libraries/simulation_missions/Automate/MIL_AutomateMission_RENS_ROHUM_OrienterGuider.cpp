// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"

#include "MIL_AutomateMission_RENS_ROHUM_OrienterGuider.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_RENS_ROHUM_OrienterGuider::nDIAAmiIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_OrienterGuider constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_ROHUM_OrienterGuider::MIL_AutomateMission_RENS_ROHUM_OrienterGuider( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_OrienterGuider destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_ROHUM_OrienterGuider::~MIL_AutomateMission_RENS_ROHUM_OrienterGuider()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_OrienterGuider::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_RENS_ROHUM_OrienterGuider::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAAmiIdx_ = DEC_Tools::InitializeDIAField( "ami_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_OrienterGuider::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_RENS_ROHUM_OrienterGuider::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider& asnMission = *asnMsg.mission.u.mission_automate_rens_rohum_orienter_guider;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.ami, GetVariable( nDIAAmiIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROHUM_OrienterGuider::Terminate()
{
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_OrienterGuider::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROHUM_OrienterGuider::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider& asnMission = *new ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_rens_rohum_orienter_guider;
    asnMsg.mission.u.mission_automate_rens_rohum_orienter_guider  = &asnMission;

    NET_ASN_Tools::CopyAutomate( GetVariable( nDIAAmiIdx_ ), asnMission.ami );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_OrienterGuider::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROHUM_OrienterGuider::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_rens_rohum_orienter_guider );
    ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider& asnMission = *asnMsg.mission.u.mission_automate_rens_rohum_orienter_guider;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
