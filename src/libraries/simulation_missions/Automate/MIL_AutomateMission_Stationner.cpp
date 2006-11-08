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

#include "simulation_missions_pch.h"

#include "MIL_AutomateMission_Stationner.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_Stationner::nDIAZoneAttenteIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Stationner constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_Stationner::MIL_AutomateMission_Stationner( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Stationner destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_Stationner::~MIL_AutomateMission_Stationner()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Stationner::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_Stationner::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneAttenteIdx_ = DEC_Tools::InitializeDIAField( "zoneAttente_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Stationner::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_Stationner::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_Stationner& asnMission = *asnMsg.mission.u.mission_automate_stationner;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_attente, GetVariable( nDIAZoneAttenteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Stationner::Terminate()
{
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Stationner::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Stationner::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_Stationner& asnMission = *new ASN1T_Mission_Automate_Stationner();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_stationner;
    asnMsg.mission.u.mission_automate_stationner  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneAttenteIdx_ ), asnMission.zone_attente );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Stationner::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Stationner::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_stationner );
    ASN1T_Mission_Automate_Stationner& asnMission = *asnMsg.mission.u.mission_automate_stationner;

    NET_ASN_Tools::Delete( asnMission.zone_attente );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
