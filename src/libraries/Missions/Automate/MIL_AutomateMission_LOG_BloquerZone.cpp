// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:43 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_LOG_BloquerZone.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_LOG_BloquerZone::nDIAZoneIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_BloquerZone constructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_LOG_BloquerZone::MIL_AutomateMission_LOG_BloquerZone( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_BloquerZone destructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_LOG_BloquerZone::~MIL_AutomateMission_LOG_BloquerZone()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_BloquerZone::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_LOG_BloquerZone::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_BloquerZone::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_LOG_BloquerZone::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_LOG_BloquerZone& asnMission = *asnMsg.mission.u.mission_automate_log_bloquer_zone;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, zone_, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_BloquerZone::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zone_, GetVariable( nDIAZoneIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_BloquerZone::Serialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_BloquerZone::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_LOG_BloquerZone& asnMission = *new ASN1T_Mission_Automate_LOG_BloquerZone();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_log_bloquer_zone;
    asnMsg.mission.u.mission_automate_log_bloquer_zone  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_BloquerZone::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_BloquerZone::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_log_bloquer_zone );
    ASN1T_Mission_Automate_LOG_BloquerZone& asnMission = *asnMsg.mission.u.mission_automate_log_bloquer_zone;

    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
