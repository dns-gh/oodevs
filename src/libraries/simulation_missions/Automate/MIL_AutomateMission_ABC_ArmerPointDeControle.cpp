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

#include "MIL_AutomateMission_ABC_ArmerPointDeControle.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_ABC_ArmerPointDeControle::nDIAZoneDeControleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ArmerPointDeControle constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_ArmerPointDeControle::MIL_AutomateMission_ABC_ArmerPointDeControle( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ArmerPointDeControle destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_ArmerPointDeControle::~MIL_AutomateMission_ABC_ArmerPointDeControle()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ArmerPointDeControle::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_ArmerPointDeControle::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneDeControleIdx_ = DEC_Tools::InitializeDIAField( "zoneDeControle_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ArmerPointDeControle::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_ArmerPointDeControle::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ABC_ArmerPointDeControle& asnMission = *asnMsg.mission.u.mission_automate_abc_armer_point_de_controle;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_de_controle, GetVariable( nDIAZoneDeControleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ArmerPointDeControle::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_ArmerPointDeControle::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_ArmerPointDeControle& asnMission = *new ASN1T_Mission_Automate_ABC_ArmerPointDeControle();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_armer_point_de_controle;
    asnMsg.mission.u.mission_automate_abc_armer_point_de_controle  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneDeControleIdx_ ), asnMission.zone_de_controle );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ArmerPointDeControle::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_ArmerPointDeControle::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_armer_point_de_controle );
    ASN1T_Mission_Automate_ABC_ArmerPointDeControle& asnMission = *asnMsg.mission.u.mission_automate_abc_armer_point_de_controle;

    NET_ASN_Tools::Delete( asnMission.zone_de_controle );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
