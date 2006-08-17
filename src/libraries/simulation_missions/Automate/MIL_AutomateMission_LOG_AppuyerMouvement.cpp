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

#include "MIL_AutomateMission_LOG_AppuyerMouvement.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_LOG_AppuyerMouvement::nDIAUnitesAAppuyerIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_AppuyerMouvement constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_LOG_AppuyerMouvement::MIL_AutomateMission_LOG_AppuyerMouvement( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_AppuyerMouvement destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_LOG_AppuyerMouvement::~MIL_AutomateMission_LOG_AppuyerMouvement()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_AppuyerMouvement::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_LOG_AppuyerMouvement::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesAAppuyerIdx_ = DEC_Tools::InitializeDIAField( "unitesAAppuyer_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_AppuyerMouvement::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_LOG_AppuyerMouvement::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_LOG_AppuyerMouvement& asnMission = *asnMsg.mission.u.mission_automate_log_appuyer_mouvement;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_appuyer, GetVariable( nDIAUnitesAAppuyerIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_AppuyerMouvement::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_AppuyerMouvement::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_AppuyerMouvement::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_LOG_AppuyerMouvement& asnMission = *new ASN1T_Mission_Automate_LOG_AppuyerMouvement();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_log_appuyer_mouvement;
    asnMsg.mission.u.mission_automate_log_appuyer_mouvement  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesAAppuyerIdx_ ), asnMission.unites_a_appuyer );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_AppuyerMouvement::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_AppuyerMouvement::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_log_appuyer_mouvement );
    ASN1T_Mission_Automate_LOG_AppuyerMouvement& asnMission = *asnMsg.mission.u.mission_automate_log_appuyer_mouvement;

    NET_ASN_Tools::Delete( asnMission.unites_a_appuyer );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
