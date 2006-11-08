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

#include "MIL_AutomateMission_INF_ReconnaissanceOffensive.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"



//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_ReconnaissanceOffensive constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_ReconnaissanceOffensive::MIL_AutomateMission_INF_ReconnaissanceOffensive( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_ReconnaissanceOffensive destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_ReconnaissanceOffensive::~MIL_AutomateMission_INF_ReconnaissanceOffensive()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_ReconnaissanceOffensive::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_ReconnaissanceOffensive::InitializeDIA( const MIL_AutomateMissionType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetDIATypeName() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_ReconnaissanceOffensive::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_ReconnaissanceOffensive::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        


    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_ReconnaissanceOffensive::Terminate()
{
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_ReconnaissanceOffensive::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_ReconnaissanceOffensive::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_ReconnaissanceOffensive& asnMission = *new ASN1T_Mission_Automate_INF_ReconnaissanceOffensive();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_reconnaissance_offensive;
    asnMsg.mission.u.mission_automate_inf_reconnaissance_offensive  = &asnMission;


}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_ReconnaissanceOffensive::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_ReconnaissanceOffensive::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_reconnaissance_offensive );
    ASN1T_Mission_Automate_INF_ReconnaissanceOffensive& asnMission = *asnMsg.mission.u.mission_automate_inf_reconnaissance_offensive;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
