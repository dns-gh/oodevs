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

#include "MIL_AutomateMission_ASA_DefendreUnites.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_ASA_DefendreUnites::nDIAUnitesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_DefendreUnites constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASA_DefendreUnites::MIL_AutomateMission_ASA_DefendreUnites( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_DefendreUnites destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASA_DefendreUnites::~MIL_AutomateMission_ASA_DefendreUnites()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_DefendreUnites::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ASA_DefendreUnites::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesIdx_ = DEC_Tools::InitializeDIAField( "unites_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_DefendreUnites::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ASA_DefendreUnites::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ASA_DefendreUnites& asnMission = *asnMsg.mission.u.mission_automate_asa_defendre_unites;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites, GetVariable( nDIAUnitesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_DefendreUnites::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_DefendreUnites::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_DefendreUnites::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ASA_DefendreUnites& asnMission = *new ASN1T_Mission_Automate_ASA_DefendreUnites();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_asa_defendre_unites;
    asnMsg.mission.u.mission_automate_asa_defendre_unites  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesIdx_ ), asnMission.unites );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_DefendreUnites::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_DefendreUnites::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_asa_defendre_unites );
    ASN1T_Mission_Automate_ASA_DefendreUnites& asnMission = *asnMsg.mission.u.mission_automate_asa_defendre_unites;

    NET_ASN_Tools::Delete( asnMission.unites );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
