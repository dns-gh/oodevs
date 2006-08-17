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

#include "MIL_AutomateMission_ASS_SeDeployer.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_ASS_SeDeployer::nDIAZoneDeploiementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_SeDeployer constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASS_SeDeployer::MIL_AutomateMission_ASS_SeDeployer( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_SeDeployer destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASS_SeDeployer::~MIL_AutomateMission_ASS_SeDeployer()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_SeDeployer::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ASS_SeDeployer::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneDeploiementIdx_ = DEC_Tools::InitializeDIAField( "zoneDeploiement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_SeDeployer::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ASS_SeDeployer::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ASS_SeDeployer& asnMission = *asnMsg.mission.u.mission_automate_ass_se_deployer;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_deploiement, GetVariable( nDIAZoneDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASS_SeDeployer::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_SeDeployer::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASS_SeDeployer::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ASS_SeDeployer& asnMission = *new ASN1T_Mission_Automate_ASS_SeDeployer();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_ass_se_deployer;
    asnMsg.mission.u.mission_automate_ass_se_deployer  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneDeploiementIdx_ ), asnMission.zone_deploiement );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_SeDeployer::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASS_SeDeployer::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_ass_se_deployer );
    ASN1T_Mission_Automate_ASS_SeDeployer& asnMission = *asnMsg.mission.u.mission_automate_ass_se_deployer;

    NET_ASN_Tools::Delete( asnMission.zone_deploiement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
