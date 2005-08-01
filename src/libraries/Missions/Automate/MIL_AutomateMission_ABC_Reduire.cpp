// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_ABC_Reduire.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ABC_Reduire::nDIAZoneResistanceIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Reduire constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_Reduire::MIL_AutomateMission_ABC_Reduire( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Reduire destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_Reduire::~MIL_AutomateMission_ABC_Reduire()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Reduire::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_Reduire::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneResistanceIdx_ = DEC_Tools::InitializeDIAField( "zoneResistance_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Reduire::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_Reduire::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ABC_Reduire& asnMission = *asnMsg.mission.u.mission_automate_abc_reduire;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_resistance, zoneResistance_, GetVariable( nDIAZoneResistanceIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Reduire::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zoneResistance_, GetVariable( nDIAZoneResistanceIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Reduire::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Reduire::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_Reduire& asnMission = *new ASN1T_Mission_Automate_ABC_Reduire();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_reduire;
    asnMsg.mission.u.mission_automate_abc_reduire  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneResistanceIdx_ ), asnMission.zone_resistance );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Reduire::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Reduire::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_reduire );
    ASN1T_Mission_Automate_ABC_Reduire& asnMission = *asnMsg.mission.u.mission_automate_abc_reduire;

    NET_ASN_Tools::Delete( asnMission.zone_resistance );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
