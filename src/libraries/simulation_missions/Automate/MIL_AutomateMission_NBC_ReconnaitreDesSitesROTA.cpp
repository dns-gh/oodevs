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

#include "MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::nDIASitesROTAIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::~MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASitesROTAIdx_ = DEC_Tools::InitializeDIAField( "sitesROTA_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA& asnMission = *asnMsg.mission.u.mission_automate_nbc_reconnaitre_des_sites_rota;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.sites_ROTA, GetVariable( nDIASitesROTAIdx_ ), automate_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA& asnMission = *new ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_nbc_reconnaitre_des_sites_rota;
    asnMsg.mission.u.mission_automate_nbc_reconnaitre_des_sites_rota  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIASitesROTAIdx_ ), asnMission.sites_ROTA, automate_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_nbc_reconnaitre_des_sites_rota );
    ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA& asnMission = *asnMsg.mission.u.mission_automate_nbc_reconnaitre_des_sites_rota;

    NET_ASN_Tools::Delete( asnMission.sites_ROTA );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
