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

#include "MIL_AutomateMission_INF_Couvrir.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_Couvrir::nDIACompagnieIdx_ = 0 ;
int MIL_AutomateMission_INF_Couvrir::nDIAObjectifsIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Couvrir constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_Couvrir::MIL_AutomateMission_INF_Couvrir( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Couvrir destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_Couvrir::~MIL_AutomateMission_INF_Couvrir()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Couvrir::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_Couvrir::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIACompagnieIdx_ = DEC_Tools::InitializeDIAField( "compagnie_", diaType );
    nDIAObjectifsIdx_ = DEC_Tools::InitializeDIAField( "objectifs_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Couvrir::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_Couvrir::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_Couvrir& asnMission = *asnMsg.mission.u.mission_automate_inf_couvrir;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.compagnie, GetVariable( nDIACompagnieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPointList( asnMission.objectifs, GetVariable( nDIAObjectifsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Couvrir::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Couvrir::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_Couvrir& asnMission = *new ASN1T_Mission_Automate_INF_Couvrir();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_couvrir;
    asnMsg.mission.u.mission_automate_inf_couvrir  = &asnMission;

    NET_ASN_Tools::CopyAutomate( GetVariable( nDIACompagnieIdx_ ), asnMission.compagnie );
    NET_ASN_Tools::CopyPointList( GetVariable( nDIAObjectifsIdx_ ), asnMission.objectifs );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Couvrir::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Couvrir::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_couvrir );
    ASN1T_Mission_Automate_INF_Couvrir& asnMission = *asnMsg.mission.u.mission_automate_inf_couvrir;

    NET_ASN_Tools::Delete( asnMission.objectifs );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
