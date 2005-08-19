// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:55 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_INF_RecueillirUnite.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_RecueillirUnite::nDIAZoneDeploiementIdx_ = 0 ;
int MIL_AutomateMission_INF_RecueillirUnite::nDIACompagnieIdx_ = 0 ;
int MIL_AutomateMission_INF_RecueillirUnite::nDIAPiasIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RecueillirUnite constructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_RecueillirUnite::MIL_AutomateMission_INF_RecueillirUnite( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RecueillirUnite destructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_RecueillirUnite::~MIL_AutomateMission_INF_RecueillirUnite()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RecueillirUnite::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_RecueillirUnite::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneDeploiementIdx_ = DEC_Tools::InitializeDIAField( "zoneDeploiement_", diaType );
    nDIACompagnieIdx_ = DEC_Tools::InitializeDIAField( "compagnie_", diaType );
    nDIAPiasIdx_ = DEC_Tools::InitializeDIAField( "pias_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RecueillirUnite::Initialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_RecueillirUnite::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_RecueillirUnite& asnMission = *asnMsg.mission.u.mission_automate_inf_recueillir_unite;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_deploiement, zoneDeploiement_, GetVariable( nDIAZoneDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.compagnie, GetVariable( nDIACompagnieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPointList( asnMission.pias, pias_, GetVariable( nDIAPiasIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_RecueillirUnite::Terminate()
{
    NET_ASN_Tools::ResetPointList( pias_, GetVariable( nDIAPiasIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RecueillirUnite::Serialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_RecueillirUnite::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_RecueillirUnite& asnMission = *new ASN1T_Mission_Automate_INF_RecueillirUnite();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_recueillir_unite;
    asnMsg.mission.u.mission_automate_inf_recueillir_unite  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneDeploiementIdx_ ), asnMission.zone_deploiement );
    NET_ASN_Tools::CopyAutomate( GetVariable( nDIACompagnieIdx_ ), asnMission.compagnie );
    NET_ASN_Tools::CopyPointList( GetVariable( nDIAPiasIdx_ ), asnMission.pias );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RecueillirUnite::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_RecueillirUnite::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_recueillir_unite );
    ASN1T_Mission_Automate_INF_RecueillirUnite& asnMission = *asnMsg.mission.u.mission_automate_inf_recueillir_unite;

    NET_ASN_Tools::Delete( asnMission.zone_deploiement );
    NET_ASN_Tools::Delete( asnMission.pias );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
