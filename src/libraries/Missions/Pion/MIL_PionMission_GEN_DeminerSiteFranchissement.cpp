// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_GEN_DeminerSiteFranchissement.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_DeminerSiteFranchissement::nDIASiteFranchissementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DeminerSiteFranchissement::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_DeminerSiteFranchissement::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASiteFranchissementIdx_ = DEC_Tools::InitializeDIAField( "siteFranchissement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DeminerSiteFranchissement constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_DeminerSiteFranchissement::MIL_PionMission_GEN_DeminerSiteFranchissement( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DeminerSiteFranchissement destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_DeminerSiteFranchissement::~MIL_PionMission_GEN_DeminerSiteFranchissement()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DeminerSiteFranchissement::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_DeminerSiteFranchissement::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement& asnMission = *asnMsg.mission.u.mission_pion_gen_deminer_site_franchissement;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.site_franchissement, GetVariable( nDIASiteFranchissementIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DeminerSiteFranchissement::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_DeminerSiteFranchissement::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetObjectKnowledge( GetVariable( nDIASiteFranchissementIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DeminerSiteFranchissement::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_DeminerSiteFranchissement::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_DeminerSiteFranchissement& mission = static_cast< MIL_PionMission_GEN_DeminerSiteFranchissement& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIASiteFranchissementIdx_ ), GetVariable( nDIASiteFranchissementIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DeminerSiteFranchissement::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DeminerSiteFranchissement::Terminate()
{
    NET_ASN_Tools::ResetObjectKnowledge( GetVariable( nDIASiteFranchissementIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DeminerSiteFranchissement::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DeminerSiteFranchissement::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement& asnMission = *new ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_deminer_site_franchissement;
    asnMsg.mission.u.mission_pion_gen_deminer_site_franchissement  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIASiteFranchissementIdx_ ), asnMission.site_franchissement, pion_.GetKnowledgeGroup().GetKSQuerier() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_DeminerSiteFranchissement::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_DeminerSiteFranchissement::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_deminer_site_franchissement );
    ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement& asnMission = *asnMsg.mission.u.mission_pion_gen_deminer_site_franchissement;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
