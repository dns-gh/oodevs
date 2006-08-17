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

#include "MIL_AutomateMission_INF_AppuyerUnFreinage.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_AppuyerUnFreinage::nDIACompagnieIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerUnFreinage constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_AppuyerUnFreinage::MIL_AutomateMission_INF_AppuyerUnFreinage( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerUnFreinage destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_AppuyerUnFreinage::~MIL_AutomateMission_INF_AppuyerUnFreinage()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerUnFreinage::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_AppuyerUnFreinage::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIACompagnieIdx_ = DEC_Tools::InitializeDIAField( "compagnie_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerUnFreinage::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_AppuyerUnFreinage::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_AppuyerUnFreinage& asnMission = *asnMsg.mission.u.mission_automate_inf_appuyer_un_freinage;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.compagnie, GetVariable( nDIACompagnieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AppuyerUnFreinage::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerUnFreinage::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AppuyerUnFreinage::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_AppuyerUnFreinage& asnMission = *new ASN1T_Mission_Automate_INF_AppuyerUnFreinage();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_appuyer_un_freinage;
    asnMsg.mission.u.mission_automate_inf_appuyer_un_freinage  = &asnMission;

    NET_ASN_Tools::CopyAutomate( GetVariable( nDIACompagnieIdx_ ), asnMission.compagnie );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerUnFreinage::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AppuyerUnFreinage::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_appuyer_un_freinage );
    ASN1T_Mission_Automate_INF_AppuyerUnFreinage& asnMission = *asnMsg.mission.u.mission_automate_inf_appuyer_un_freinage;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
