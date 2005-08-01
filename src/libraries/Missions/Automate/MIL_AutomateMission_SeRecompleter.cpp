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

#include "MIL_AutomateMission_SeRecompleter.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_SeRecompleter::nDIAZoneRavitaillementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeRecompleter constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_SeRecompleter::MIL_AutomateMission_SeRecompleter( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeRecompleter destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_SeRecompleter::~MIL_AutomateMission_SeRecompleter()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeRecompleter::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_SeRecompleter::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "zoneRavitaillement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeRecompleter::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_SeRecompleter::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_SeRecompleter& asnMission = *asnMsg.mission.u.mission_automate_se_recompleter;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_ravitaillement, zoneRavitaillement_, GetVariable( nDIAZoneRavitaillementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_SeRecompleter::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zoneRavitaillement_, GetVariable( nDIAZoneRavitaillementIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeRecompleter::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_SeRecompleter::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_SeRecompleter& asnMission = *new ASN1T_Mission_Automate_SeRecompleter();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_se_recompleter;
    asnMsg.mission.u.mission_automate_se_recompleter  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneRavitaillementIdx_ ), asnMission.zone_ravitaillement );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeRecompleter::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_SeRecompleter::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_se_recompleter );
    ASN1T_Mission_Automate_SeRecompleter& asnMission = *asnMsg.mission.u.mission_automate_se_recompleter;

    NET_ASN_Tools::Delete( asnMission.zone_ravitaillement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
