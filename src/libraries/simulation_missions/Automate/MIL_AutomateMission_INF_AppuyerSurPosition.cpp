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

#include "MIL_AutomateMission_INF_AppuyerSurPosition.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_AppuyerSurPosition::nDIAPositionInstallationIdx_ = 0 ;
int MIL_AutomateMission_INF_AppuyerSurPosition::nDIACompagnieIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerSurPosition constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_AppuyerSurPosition::MIL_AutomateMission_INF_AppuyerSurPosition( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerSurPosition destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_AppuyerSurPosition::~MIL_AutomateMission_INF_AppuyerSurPosition()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerSurPosition::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_AppuyerSurPosition::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionInstallationIdx_ = DEC_Tools::InitializeDIAField( "positionInstallation_", diaType );
    nDIACompagnieIdx_ = DEC_Tools::InitializeDIAField( "compagnie_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerSurPosition::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_AppuyerSurPosition::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_AppuyerSurPosition& asnMission = *asnMsg.mission.u.mission_automate_inf_appuyer_sur_position;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.position_installation, GetVariable( nDIAPositionInstallationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.compagnie, GetVariable( nDIACompagnieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AppuyerSurPosition::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerSurPosition::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AppuyerSurPosition::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_AppuyerSurPosition& asnMission = *new ASN1T_Mission_Automate_INF_AppuyerSurPosition();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_appuyer_sur_position;
    asnMsg.mission.u.mission_automate_inf_appuyer_sur_position  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAPositionInstallationIdx_ ), asnMission.position_installation );
    NET_ASN_Tools::CopyAutomate( GetVariable( nDIACompagnieIdx_ ), asnMission.compagnie );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AppuyerSurPosition::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AppuyerSurPosition::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_appuyer_sur_position );
    ASN1T_Mission_Automate_INF_AppuyerSurPosition& asnMission = *asnMsg.mission.u.mission_automate_inf_appuyer_sur_position;

    NET_ASN_Tools::Delete( asnMission.position_installation );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
