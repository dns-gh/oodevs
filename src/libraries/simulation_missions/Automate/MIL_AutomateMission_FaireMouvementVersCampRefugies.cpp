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

#include "MIL_AutomateMission_FaireMouvementVersCampRefugies.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"



//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FaireMouvementVersCampRefugies constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_FaireMouvementVersCampRefugies::MIL_AutomateMission_FaireMouvementVersCampRefugies( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FaireMouvementVersCampRefugies destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_FaireMouvementVersCampRefugies::~MIL_AutomateMission_FaireMouvementVersCampRefugies()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FaireMouvementVersCampRefugies::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_FaireMouvementVersCampRefugies::InitializeDIA( const MIL_AutomateMissionType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetDIATypeName() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FaireMouvementVersCampRefugies::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_FaireMouvementVersCampRefugies::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
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
// Name: MIL_AutomateMission_FaireMouvementVersCampRefugies::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_FaireMouvementVersCampRefugies::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_FaireMouvementVersCampRefugies& asnMission = *new ASN1T_Mission_Automate_FaireMouvementVersCampRefugies();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_faire_mouvement_vers_camp_refugies;
    asnMsg.mission.u.mission_automate_faire_mouvement_vers_camp_refugies  = &asnMission;


}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FaireMouvementVersCampRefugies::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_FaireMouvementVersCampRefugies::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_faire_mouvement_vers_camp_refugies );
    ASN1T_Mission_Automate_FaireMouvementVersCampRefugies& asnMission = *asnMsg.mission.u.mission_automate_faire_mouvement_vers_camp_refugies;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
