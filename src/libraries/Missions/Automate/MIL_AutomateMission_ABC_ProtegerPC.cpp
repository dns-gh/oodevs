// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:43 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_ABC_ProtegerPC.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ABC_ProtegerPC::nDIAPcIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ProtegerPC constructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_ProtegerPC::MIL_AutomateMission_ABC_ProtegerPC( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ProtegerPC destructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_ProtegerPC::~MIL_AutomateMission_ABC_ProtegerPC()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ProtegerPC::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_ProtegerPC::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPcIdx_ = DEC_Tools::InitializeDIAField( "pc_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ProtegerPC::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_ProtegerPC::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ABC_ProtegerPC& asnMission = *asnMsg.mission.u.mission_automate_abc_proteger_pc;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.pc, GetVariable( nDIAPcIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_ProtegerPC::Terminate()
{
    NET_ASN_Tools::ResetAutomate( GetVariable( nDIAPcIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ProtegerPC::Serialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_ProtegerPC::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_ProtegerPC& asnMission = *new ASN1T_Mission_Automate_ABC_ProtegerPC();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_proteger_pc;
    asnMsg.mission.u.mission_automate_abc_proteger_pc  = &asnMission;

    NET_ASN_Tools::CopyAutomate( GetVariable( nDIAPcIdx_ ), asnMission.pc );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ProtegerPC::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_ProtegerPC::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_proteger_pc );
    ASN1T_Mission_Automate_ABC_ProtegerPC& asnMission = *asnMsg.mission.u.mission_automate_abc_proteger_pc;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
