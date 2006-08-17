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

#include "MIL_AutomateMission_RENS_RADINT_RenseignerSur.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_RENS_RADINT_RenseignerSur::nDIAZoneIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_RADINT_RenseignerSur constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_RADINT_RenseignerSur::MIL_AutomateMission_RENS_RADINT_RenseignerSur( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_RADINT_RenseignerSur destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_RADINT_RenseignerSur::~MIL_AutomateMission_RENS_RADINT_RenseignerSur()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_RADINT_RenseignerSur::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_RENS_RADINT_RenseignerSur::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_RADINT_RenseignerSur::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_RENS_RADINT_RenseignerSur::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur& asnMission = *asnMsg.mission.u.mission_automate_rens_radint_renseignersur;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_RADINT_RenseignerSur::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_RADINT_RenseignerSur::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_RADINT_RenseignerSur::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur& asnMission = *new ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_rens_radint_renseignersur;
    asnMsg.mission.u.mission_automate_rens_radint_renseignersur  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_RADINT_RenseignerSur::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_RADINT_RenseignerSur::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_rens_radint_renseignersur );
    ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur& asnMission = *asnMsg.mission.u.mission_automate_rens_radint_renseignersur;

    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
