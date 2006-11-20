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

#include "MIL_AutomateMission_ABC_PrendrePreciserContact.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"



//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_PrendrePreciserContact constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_PrendrePreciserContact::MIL_AutomateMission_ABC_PrendrePreciserContact( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_PrendrePreciserContact destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_PrendrePreciserContact::~MIL_AutomateMission_ABC_PrendrePreciserContact()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_PrendrePreciserContact::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_PrendrePreciserContact::InitializeDIA( const MIL_AutomateMissionType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetDIATypeName() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_PrendrePreciserContact::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_PrendrePreciserContact::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
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
// Name: MIL_AutomateMission_ABC_PrendrePreciserContact::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_PrendrePreciserContact::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_PrendrePreciserContact& asnMission = *new ASN1T_Mission_Automate_ABC_PrendrePreciserContact();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_prendre_preciser_contact;
    asnMsg.mission.u.mission_automate_abc_prendre_preciser_contact  = &asnMission;


}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_PrendrePreciserContact::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_PrendrePreciserContact::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_prendre_preciser_contact );
    ASN1T_Mission_Automate_ABC_PrendrePreciserContact& asnMission = *asnMsg.mission.u.mission_automate_abc_prendre_preciser_contact;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
