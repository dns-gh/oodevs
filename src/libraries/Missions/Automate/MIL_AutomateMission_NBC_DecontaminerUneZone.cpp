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

#include "MIL_AutomateMission_NBC_DecontaminerUneZone.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_NBC_DecontaminerUneZone::nDIAZoneIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_DecontaminerUneZone constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_AutomateMission_NBC_DecontaminerUneZone::MIL_AutomateMission_NBC_DecontaminerUneZone( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_DecontaminerUneZone destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_AutomateMission_NBC_DecontaminerUneZone::~MIL_AutomateMission_NBC_DecontaminerUneZone()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_DecontaminerUneZone::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_NBC_DecontaminerUneZone::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_DecontaminerUneZone::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_NBC_DecontaminerUneZone::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_NBC_DecontaminerUneZone& asnMission = *asnMsg.mission.u.mission_automate_nbc_decontaminer_une_zone;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, zone_, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_AutomateMission_NBC_DecontaminerUneZone::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zone_, GetVariable( nDIAZoneIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_DecontaminerUneZone::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_AutomateMission_NBC_DecontaminerUneZone::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_NBC_DecontaminerUneZone& asnMission = *new ASN1T_Mission_Automate_NBC_DecontaminerUneZone();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_nbc_decontaminer_une_zone;
    asnMsg.mission.u.mission_automate_nbc_decontaminer_une_zone  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_DecontaminerUneZone::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_AutomateMission_NBC_DecontaminerUneZone::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_nbc_decontaminer_une_zone );
    ASN1T_Mission_Automate_NBC_DecontaminerUneZone& asnMission = *asnMsg.mission.u.mission_automate_nbc_decontaminer_une_zone;

    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
