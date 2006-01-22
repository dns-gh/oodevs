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

#include "Missions_pch.h"

#include "MIL_AutomateMission_INF_EscorterUnite.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_EscorterUnite::nDIAUniteAEscorterIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnite constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_EscorterUnite::MIL_AutomateMission_INF_EscorterUnite( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnite destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_EscorterUnite::~MIL_AutomateMission_INF_EscorterUnite()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnite::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_EscorterUnite::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUniteAEscorterIdx_ = DEC_Tools::InitializeDIAField( "uniteAEscorter_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnite::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_EscorterUnite::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_EscorterUnite& asnMission = *asnMsg.mission.u.mission_automate_inf_escorter_unite;
    if( !NET_ASN_Tools::CopyAgent( asnMission.unite_a_escorter, GetVariable( nDIAUniteAEscorterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_EscorterUnite::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnite::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_EscorterUnite::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_EscorterUnite& asnMission = *new ASN1T_Mission_Automate_INF_EscorterUnite();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_escorter_unite;
    asnMsg.mission.u.mission_automate_inf_escorter_unite  = &asnMission;

    NET_ASN_Tools::CopyAgent( GetVariable( nDIAUniteAEscorterIdx_ ), asnMission.unite_a_escorter );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_EscorterUnite::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_EscorterUnite::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_escorter_unite );
    ASN1T_Mission_Automate_INF_EscorterUnite& asnMission = *asnMsg.mission.u.mission_automate_inf_escorter_unite;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
