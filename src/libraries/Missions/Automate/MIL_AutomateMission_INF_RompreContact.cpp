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

#include "MIL_AutomateMission_INF_RompreContact.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_RompreContact::nDIAZoneRegroupementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RompreContact constructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_RompreContact::MIL_AutomateMission_INF_RompreContact( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RompreContact destructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_RompreContact::~MIL_AutomateMission_INF_RompreContact()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RompreContact::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_RompreContact::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneRegroupementIdx_ = DEC_Tools::InitializeDIAField( "zoneRegroupement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RompreContact::Initialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_RompreContact::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_RompreContact& asnMission = *asnMsg.mission.u.mission_automate_inf_rompre_contact;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_regroupement, zoneRegroupement_, GetVariable( nDIAZoneRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_RompreContact::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zoneRegroupement_, GetVariable( nDIAZoneRegroupementIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RompreContact::Serialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_RompreContact::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_RompreContact& asnMission = *new ASN1T_Mission_Automate_INF_RompreContact();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_rompre_contact;
    asnMsg.mission.u.mission_automate_inf_rompre_contact  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneRegroupementIdx_ ), asnMission.zone_regroupement );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_RompreContact::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_RompreContact::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_rompre_contact );
    ASN1T_Mission_Automate_INF_RompreContact& asnMission = *asnMsg.mission.u.mission_automate_inf_rompre_contact;

    NET_ASN_Tools::Delete( asnMission.zone_regroupement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
