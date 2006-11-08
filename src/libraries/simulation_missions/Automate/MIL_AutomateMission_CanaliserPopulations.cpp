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

#include "MIL_AutomateMission_CanaliserPopulations.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_CanaliserPopulations::nDIAZonesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_CanaliserPopulations constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_CanaliserPopulations::MIL_AutomateMission_CanaliserPopulations( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_CanaliserPopulations destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_CanaliserPopulations::~MIL_AutomateMission_CanaliserPopulations()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_CanaliserPopulations::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_CanaliserPopulations::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZonesIdx_ = DEC_Tools::InitializeDIAField( "zones_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_CanaliserPopulations::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_CanaliserPopulations::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_CanaliserPopulations& asnMission = *asnMsg.mission.u.mission_automate_canaliser_populations;
    if( !NET_ASN_Tools::CopyLocationList( asnMission.zones, GetVariable( nDIAZonesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_CanaliserPopulations::Terminate()
{
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_CanaliserPopulations::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_CanaliserPopulations::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_CanaliserPopulations& asnMission = *new ASN1T_Mission_Automate_CanaliserPopulations();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_canaliser_populations;
    asnMsg.mission.u.mission_automate_canaliser_populations  = &asnMission;

    NET_ASN_Tools::CopyLocationList( GetVariable( nDIAZonesIdx_ ), asnMission.zones );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_CanaliserPopulations::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_CanaliserPopulations::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_canaliser_populations );
    ASN1T_Mission_Automate_CanaliserPopulations& asnMission = *asnMsg.mission.u.mission_automate_canaliser_populations;

    NET_ASN_Tools::Delete( asnMission.zones );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
