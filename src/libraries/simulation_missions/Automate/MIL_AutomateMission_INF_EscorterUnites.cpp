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

#include "MIL_AutomateMission_INF_EscorterUnites.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_EscorterUnites::nDIAAutomateAEscorterIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnites constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_EscorterUnites::MIL_AutomateMission_INF_EscorterUnites( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnites destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_EscorterUnites::~MIL_AutomateMission_INF_EscorterUnites()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnites::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_EscorterUnites::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAAutomateAEscorterIdx_ = DEC_Tools::InitializeDIAField( "automateAEscorter_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnites::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_EscorterUnites::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_EscorterUnites& asnMission = *asnMsg.mission.u.mission_automate_inf_escorter_unites;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.automate_a_escorter, GetVariable( nDIAAutomateAEscorterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnites::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_EscorterUnites::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_EscorterUnites& asnMission = *new ASN1T_Mission_Automate_INF_EscorterUnites();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_escorter_unites;
    asnMsg.mission.u.mission_automate_inf_escorter_unites  = &asnMission;

    NET_ASN_Tools::CopyAutomate( GetVariable( nDIAAutomateAEscorterIdx_ ), asnMission.automate_a_escorter );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnites::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_EscorterUnites::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_escorter_unites );
    ASN1T_Mission_Automate_INF_EscorterUnites& asnMission = *asnMsg.mission.u.mission_automate_inf_escorter_unites;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
