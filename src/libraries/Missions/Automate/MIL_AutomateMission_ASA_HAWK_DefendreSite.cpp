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

#include "MIL_AutomateMission_ASA_HAWK_DefendreSite.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ASA_HAWK_DefendreSite::nDIAZoneIdx_ = 0 ;
int MIL_AutomateMission_ASA_HAWK_DefendreSite::nDIAPositionsSectionsIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_HAWK_DefendreSite constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASA_HAWK_DefendreSite::MIL_AutomateMission_ASA_HAWK_DefendreSite( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_HAWK_DefendreSite destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASA_HAWK_DefendreSite::~MIL_AutomateMission_ASA_HAWK_DefendreSite()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_HAWK_DefendreSite::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ASA_HAWK_DefendreSite::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );
    nDIAPositionsSectionsIdx_ = DEC_Tools::InitializeDIAField( "positionsSections_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_HAWK_DefendreSite::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ASA_HAWK_DefendreSite::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ASA_HAWK_DefendreSite& asnMission = *asnMsg.mission.u.mission_automate_asa_hawk_defendre_site;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPointList( asnMission.positions_sections, GetVariable( nDIAPositionsSectionsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_HAWK_DefendreSite::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_HAWK_DefendreSite::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_HAWK_DefendreSite::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ASA_HAWK_DefendreSite& asnMission = *new ASN1T_Mission_Automate_ASA_HAWK_DefendreSite();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_asa_hawk_defendre_site;
    asnMsg.mission.u.mission_automate_asa_hawk_defendre_site  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );
    NET_ASN_Tools::CopyPointList( GetVariable( nDIAPositionsSectionsIdx_ ), asnMission.positions_sections );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_HAWK_DefendreSite::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_HAWK_DefendreSite::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_asa_hawk_defendre_site );
    ASN1T_Mission_Automate_ASA_HAWK_DefendreSite& asnMission = *asnMsg.mission.u.mission_automate_asa_hawk_defendre_site;

    NET_ASN_Tools::Delete( asnMission.zone );
    NET_ASN_Tools::Delete( asnMission.positions_sections );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
