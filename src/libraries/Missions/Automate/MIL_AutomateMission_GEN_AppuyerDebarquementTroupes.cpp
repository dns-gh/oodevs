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

#include "MIL_AutomateMission_GEN_AppuyerDebarquementTroupes.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::nDIAZonesDebarquementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AppuyerDebarquementTroupes constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::MIL_AutomateMission_GEN_AppuyerDebarquementTroupes( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AppuyerDebarquementTroupes destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::~MIL_AutomateMission_GEN_AppuyerDebarquementTroupes()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZonesDebarquementIdx_ = DEC_Tools::InitializeDIAField( "zonesDebarquement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes& asnMission = *asnMsg.mission.u.mission_automate_gen_appuyer_debarquement_troupes;
    if( !NET_ASN_Tools::CopyGenObjectList( asnMission.zones_debarquement, GetVariable( nDIAZonesDebarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::Terminate()
{
    NET_ASN_Tools::ResetGenObjectList( GetVariable( nDIAZonesDebarquementIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes& asnMission = *new ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_appuyer_debarquement_troupes;
    asnMsg.mission.u.mission_automate_gen_appuyer_debarquement_troupes  = &asnMission;

    NET_ASN_Tools::CopyGenObjectList( GetVariable( nDIAZonesDebarquementIdx_ ), asnMission.zones_debarquement );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_AppuyerDebarquementTroupes::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_appuyer_debarquement_troupes );
    ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes& asnMission = *asnMsg.mission.u.mission_automate_gen_appuyer_debarquement_troupes;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
