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

#include "MIL_AutomateMission_Franchir.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_Franchir::nDIAPontFlottantIdx_ = 0 ;
int MIL_AutomateMission_Franchir::nDIAZoneArriveeIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Franchir constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_Franchir::MIL_AutomateMission_Franchir( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Franchir destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_Franchir::~MIL_AutomateMission_Franchir()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Franchir::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_Franchir::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPontFlottantIdx_ = DEC_Tools::InitializeDIAField( "pontFlottant_", diaType );
    nDIAZoneArriveeIdx_ = DEC_Tools::InitializeDIAField( "zoneArrivee_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Franchir::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_Franchir::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_Franchir& asnMission = *asnMsg.mission.u.mission_automate_franchir;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.pont_flottant, GetVariable( nDIAPontFlottantIdx_ ), automate_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_arrivee, GetVariable( nDIAZoneArriveeIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Franchir::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Franchir::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Franchir::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_Franchir& asnMission = *new ASN1T_Mission_Automate_Franchir();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_franchir;
    asnMsg.mission.u.mission_automate_franchir  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIAPontFlottantIdx_ ), asnMission.pont_flottant, automate_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneArriveeIdx_ ), asnMission.zone_arrivee );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Franchir::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Franchir::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_franchir );
    ASN1T_Mission_Automate_Franchir& asnMission = *asnMsg.mission.u.mission_automate_franchir;

    NET_ASN_Tools::Delete( asnMission.zone_arrivee );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
