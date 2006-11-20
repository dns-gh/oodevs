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

#include "MIL_AutomateMission_RENS_ROHUM_SExfiltrer.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_RENS_ROHUM_SExfiltrer::nDIAZoneDeRegroupementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_ROHUM_SExfiltrer::MIL_AutomateMission_RENS_ROHUM_SExfiltrer( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_ROHUM_SExfiltrer::~MIL_AutomateMission_RENS_ROHUM_SExfiltrer()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_RENS_ROHUM_SExfiltrer::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneDeRegroupementIdx_ = DEC_Tools::InitializeDIAField( "zoneDeRegroupement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_RENS_ROHUM_SExfiltrer::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& asnMission = *asnMsg.mission.u.mission_automate_rens_rohum_sexfiltrer;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_de_regroupement, GetVariable( nDIAZoneDeRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROHUM_SExfiltrer::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& asnMission = *new ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_rens_rohum_sexfiltrer;
    asnMsg.mission.u.mission_automate_rens_rohum_sexfiltrer  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneDeRegroupementIdx_ ), asnMission.zone_de_regroupement );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROHUM_SExfiltrer::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_rens_rohum_sexfiltrer );
    ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& asnMission = *asnMsg.mission.u.mission_automate_rens_rohum_sexfiltrer;

    NET_ASN_Tools::Delete( asnMission.zone_de_regroupement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
