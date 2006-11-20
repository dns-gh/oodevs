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

#include "MIL_AutomateMission_INF_Semparer.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_Semparer::nDIAObjectifIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Semparer constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_Semparer::MIL_AutomateMission_INF_Semparer( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Semparer destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_Semparer::~MIL_AutomateMission_INF_Semparer()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Semparer::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_Semparer::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAObjectifIdx_ = DEC_Tools::InitializeDIAField( "objectif_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Semparer::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_Semparer::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_Semparer& asnMission = *asnMsg.mission.u.mission_automate_inf_semparer;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.objectif, GetVariable( nDIAObjectifIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Semparer::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Semparer::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_Semparer& asnMission = *new ASN1T_Mission_Automate_INF_Semparer();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_semparer;
    asnMsg.mission.u.mission_automate_inf_semparer  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAObjectifIdx_ ), asnMission.objectif );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Semparer::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Semparer::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_semparer );
    ASN1T_Mission_Automate_INF_Semparer& asnMission = *asnMsg.mission.u.mission_automate_inf_semparer;

    NET_ASN_Tools::Delete( asnMission.objectif );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
