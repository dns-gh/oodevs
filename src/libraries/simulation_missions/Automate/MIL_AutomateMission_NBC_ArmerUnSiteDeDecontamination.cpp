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

#include "MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::nDIASiteIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::~MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASiteIdx_ = DEC_Tools::InitializeDIAField( "site_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& asnMission = *asnMsg.mission.u.mission_automate_nbc_armer_un_site_de_decontamination;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.site, GetVariable( nDIASiteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& asnMission = *new ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_nbc_armer_un_site_de_decontamination;
    asnMsg.mission.u.mission_automate_nbc_armer_un_site_de_decontamination  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIASiteIdx_ ), asnMission.site );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_nbc_armer_un_site_de_decontamination );
    ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& asnMission = *asnMsg.mission.u.mission_automate_nbc_armer_un_site_de_decontamination;

    NET_ASN_Tools::Delete( asnMission.site );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
