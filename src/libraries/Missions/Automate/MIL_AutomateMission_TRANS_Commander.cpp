// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_TRANS_Commander.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_TRANS_Commander::nDIAZoneDeploiementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_TRANS_Commander constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_AutomateMission_TRANS_Commander::MIL_AutomateMission_TRANS_Commander( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_TRANS_Commander destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_AutomateMission_TRANS_Commander::~MIL_AutomateMission_TRANS_Commander()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_TRANS_Commander::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_TRANS_Commander::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneDeploiementIdx_ = DEC_Tools::InitializeDIAField( "zoneDeploiement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_TRANS_Commander::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_TRANS_Commander::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_TRANS_Commander& asnMission = *asnMsg.mission.u.mission_automate_trans_commander;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_deploiement, zoneDeploiement_, GetVariable( nDIAZoneDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_AutomateMission_TRANS_Commander::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zoneDeploiement_, GetVariable( nDIAZoneDeploiementIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_TRANS_Commander::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_AutomateMission_TRANS_Commander::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_TRANS_Commander& asnMission = *new ASN1T_Mission_Automate_TRANS_Commander();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_trans_commander;
    asnMsg.mission.u.mission_automate_trans_commander  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneDeploiementIdx_ ), asnMission.zone_deploiement );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_TRANS_Commander::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_AutomateMission_TRANS_Commander::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_trans_commander );
    ASN1T_Mission_Automate_TRANS_Commander& asnMission = *asnMsg.mission.u.mission_automate_trans_commander;

    NET_ASN_Tools::Delete( asnMission.zone_deploiement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
