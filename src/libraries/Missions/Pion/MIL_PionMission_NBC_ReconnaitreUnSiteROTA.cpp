// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_NBC_ReconnaitreUnSiteROTA.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_NBC_ReconnaitreUnSiteROTA::nDIASiteROTAIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreUnSiteROTA::InitializeDIA
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_NBC_ReconnaitreUnSiteROTA::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASiteROTAIdx_ = DEC_Tools::InitializeDIAField( "siteROTA_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreUnSiteROTA constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_NBC_ReconnaitreUnSiteROTA::MIL_PionMission_NBC_ReconnaitreUnSiteROTA( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreUnSiteROTA destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_NBC_ReconnaitreUnSiteROTA::~MIL_PionMission_NBC_ReconnaitreUnSiteROTA()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreUnSiteROTA::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_NBC_ReconnaitreUnSiteROTA::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA& asnMission = *asnMsg.mission.u.mission_pion_nbc_reconnaitre_un_site_rota;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.site_ROTA, GetVariable( nDIASiteROTAIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreUnSiteROTA::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
bool MIL_PionMission_NBC_ReconnaitreUnSiteROTA::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetObjectKnowledge( GetVariable( nDIASiteROTAIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreUnSiteROTA::Initialize
// Created: 2005-08-01 - 11:23:53
// -----------------------------------------------------------------------------
bool MIL_PionMission_NBC_ReconnaitreUnSiteROTA::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_NBC_ReconnaitreUnSiteROTA& mission = static_cast< MIL_PionMission_NBC_ReconnaitreUnSiteROTA& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIASiteROTAIdx_ ), GetVariable( nDIASiteROTAIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreUnSiteROTA::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_NBC_ReconnaitreUnSiteROTA::Terminate()
{
    NET_ASN_Tools::ResetObjectKnowledge( GetVariable( nDIASiteROTAIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreUnSiteROTA::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_NBC_ReconnaitreUnSiteROTA::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA& asnMission = *new ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_nbc_reconnaitre_un_site_rota;
    asnMsg.mission.u.mission_pion_nbc_reconnaitre_un_site_rota  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIASiteROTAIdx_ ), asnMission.site_ROTA, pion_.GetKnowledgeGroup().GetKSQuerier() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreUnSiteROTA::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_NBC_ReconnaitreUnSiteROTA::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_nbc_reconnaitre_un_site_rota );
    ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA& asnMission = *asnMsg.mission.u.mission_pion_nbc_reconnaitre_un_site_rota;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
