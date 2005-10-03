// *****************************************************************************
//
// $Created: 
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_GEN_ReconnaitreSiteFranchissement.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_ReconnaitreSiteFranchissement::nDIASiteFranchissementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReconnaitreSiteFranchissement::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_ReconnaitreSiteFranchissement::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASiteFranchissementIdx_ = DEC_Tools::InitializeDIAField( "siteFranchissement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReconnaitreSiteFranchissement constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_ReconnaitreSiteFranchissement::MIL_PionMission_GEN_ReconnaitreSiteFranchissement( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReconnaitreSiteFranchissement destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_ReconnaitreSiteFranchissement::~MIL_PionMission_GEN_ReconnaitreSiteFranchissement()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReconnaitreSiteFranchissement::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_ReconnaitreSiteFranchissement::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_ReconnaitreSiteFranchissement& asnMission = *asnMsg.mission.u.mission_pion_gen_reconnaitre_site_franchissement;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.site_franchissement, GetVariable( nDIASiteFranchissementIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReconnaitreSiteFranchissement::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_ReconnaitreSiteFranchissement::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetObjectKnowledge( GetVariable( nDIASiteFranchissementIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReconnaitreSiteFranchissement::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_ReconnaitreSiteFranchissement::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_ReconnaitreSiteFranchissement& mission = static_cast< MIL_PionMission_GEN_ReconnaitreSiteFranchissement& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIASiteFranchissementIdx_ ), GetVariable( nDIASiteFranchissementIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReconnaitreSiteFranchissement::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ReconnaitreSiteFranchissement::Terminate()
{
    NET_ASN_Tools::ResetObjectKnowledge( GetVariable( nDIASiteFranchissementIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReconnaitreSiteFranchissement::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ReconnaitreSiteFranchissement::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_ReconnaitreSiteFranchissement& asnMission = *new ASN1T_Mission_Pion_GEN_ReconnaitreSiteFranchissement();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_reconnaitre_site_franchissement;
    asnMsg.mission.u.mission_pion_gen_reconnaitre_site_franchissement  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIASiteFranchissementIdx_ ), asnMission.site_franchissement, pion_.GetKnowledgeGroup().GetKSQuerier() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReconnaitreSiteFranchissement::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ReconnaitreSiteFranchissement::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_reconnaitre_site_franchissement );
    ASN1T_Mission_Pion_GEN_ReconnaitreSiteFranchissement& asnMission = *asnMsg.mission.u.mission_pion_gen_reconnaitre_site_franchissement;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
