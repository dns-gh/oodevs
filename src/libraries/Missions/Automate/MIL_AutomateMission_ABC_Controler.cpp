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

#include "MIL_AutomateMission_ABC_Controler.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ABC_Controler::nDIAZoneAControlerIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Controler constructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_Controler::MIL_AutomateMission_ABC_Controler( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Controler destructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_Controler::~MIL_AutomateMission_ABC_Controler()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Controler::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_Controler::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneAControlerIdx_ = DEC_Tools::InitializeDIAField( "zoneAControler_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Controler::Initialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_Controler::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ABC_Controler& asnMission = *asnMsg.mission.u.mission_automate_abc_controler;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_a_controler, zoneAControler_, GetVariable( nDIAZoneAControlerIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Controler::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zoneAControler_, GetVariable( nDIAZoneAControlerIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Controler::Serialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Controler::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_Controler& asnMission = *new ASN1T_Mission_Automate_ABC_Controler();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_controler;
    asnMsg.mission.u.mission_automate_abc_controler  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneAControlerIdx_ ), asnMission.zone_a_controler );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Controler::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Controler::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_controler );
    ASN1T_Mission_Automate_ABC_Controler& asnMission = *asnMsg.mission.u.mission_automate_abc_controler;

    NET_ASN_Tools::Delete( asnMission.zone_a_controler );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
