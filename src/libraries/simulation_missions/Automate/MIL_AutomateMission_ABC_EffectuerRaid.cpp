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

#include "MIL_AutomateMission_ABC_EffectuerRaid.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"



//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EffectuerRaid constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_EffectuerRaid::MIL_AutomateMission_ABC_EffectuerRaid( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EffectuerRaid destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_EffectuerRaid::~MIL_AutomateMission_ABC_EffectuerRaid()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EffectuerRaid::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_EffectuerRaid::InitializeDIA( const MIL_AutomateMissionType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetDIATypeName() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EffectuerRaid::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_EffectuerRaid::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        


    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EffectuerRaid::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_EffectuerRaid::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_EffectuerRaid& asnMission = *new ASN1T_Mission_Automate_ABC_EffectuerRaid();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_effectuer_raid;
    asnMsg.mission.u.mission_automate_abc_effectuer_raid  = &asnMission;


}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EffectuerRaid::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_EffectuerRaid::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_effectuer_raid );
    ASN1T_Mission_Automate_ABC_EffectuerRaid& asnMission = *asnMsg.mission.u.mission_automate_abc_effectuer_raid;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
