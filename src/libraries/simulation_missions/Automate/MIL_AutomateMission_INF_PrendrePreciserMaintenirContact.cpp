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

#include "MIL_AutomateMission_INF_PrendrePreciserMaintenirContact.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"



//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_PrendrePreciserMaintenirContact constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_PrendrePreciserMaintenirContact::MIL_AutomateMission_INF_PrendrePreciserMaintenirContact( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_PrendrePreciserMaintenirContact destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_PrendrePreciserMaintenirContact::~MIL_AutomateMission_INF_PrendrePreciserMaintenirContact()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_PrendrePreciserMaintenirContact::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_PrendrePreciserMaintenirContact::InitializeDIA( const MIL_AutomateMissionType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetDIATypeName() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_PrendrePreciserMaintenirContact::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_PrendrePreciserMaintenirContact::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
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
void MIL_AutomateMission_INF_PrendrePreciserMaintenirContact::Terminate()
{

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_PrendrePreciserMaintenirContact::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_PrendrePreciserMaintenirContact::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact& asnMission = *new ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_prendre_preciser_maintenir_contact;
    asnMsg.mission.u.mission_automate_inf_prendre_preciser_maintenir_contact  = &asnMission;


}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_PrendrePreciserMaintenirContact::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_PrendrePreciserMaintenirContact::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_prendre_preciser_maintenir_contact );
    ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact& asnMission = *asnMsg.mission.u.mission_automate_inf_prendre_preciser_maintenir_contact;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
