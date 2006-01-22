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
#include "MIL_PionMission_GEN_ExecuterVariantement.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_ExecuterVariantement::nDIASiteFranchissementIdx_ = 0 ;
int MIL_PionMission_GEN_ExecuterVariantement::nDIAOidObjetPlanifieIdx_ = 0 ;
int MIL_PionMission_GEN_ExecuterVariantement::nDIAPointRegroupementIdx_ = 0 ;
int MIL_PionMission_GEN_ExecuterVariantement::nDIATypePontageIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ExecuterVariantement::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_ExecuterVariantement::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASiteFranchissementIdx_ = DEC_Tools::InitializeDIAField( "siteFranchissement_", diaType );
    nDIAOidObjetPlanifieIdx_ = DEC_Tools::InitializeDIAField( "oidObjetPlanifie_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIATypePontageIdx_ = DEC_Tools::InitializeDIAField( "typePontage_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ExecuterVariantement constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_ExecuterVariantement::MIL_PionMission_GEN_ExecuterVariantement( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ExecuterVariantement destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_ExecuterVariantement::~MIL_PionMission_GEN_ExecuterVariantement()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ExecuterVariantement::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_ExecuterVariantement::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_ExecuterVariantement& asnMission = *asnMsg.mission.u.mission_pion_gen_executer_variantement;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.site_franchissement, GetVariable( nDIASiteFranchissementIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyID( asnMission.oid_objet_planifie, GetVariable( nDIAOidObjetPlanifieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.type_pontage, GetVariable( nDIATypePontageIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ExecuterVariantement::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_ExecuterVariantement::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

                
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ExecuterVariantement::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_ExecuterVariantement::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_ExecuterVariantement& mission = static_cast< MIL_PionMission_GEN_ExecuterVariantement& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIASiteFranchissementIdx_ ), GetVariable( nDIASiteFranchissementIdx_ ) );
    NET_ASN_Tools::CopyID( mission.GetVariable( nDIAOidObjetPlanifieIdx_ ), GetVariable( nDIAOidObjetPlanifieIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRegroupementIdx_ ), GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIATypePontageIdx_ ), GetVariable( nDIATypePontageIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ExecuterVariantement::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ExecuterVariantement::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ExecuterVariantement::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ExecuterVariantement::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_ExecuterVariantement& asnMission = *new ASN1T_Mission_Pion_GEN_ExecuterVariantement();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_executer_variantement;
    asnMsg.mission.u.mission_pion_gen_executer_variantement  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIASiteFranchissementIdx_ ), asnMission.site_franchissement, pion_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyID( GetVariable( nDIAOidObjetPlanifieIdx_ ), asnMission.oid_objet_planifie );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIATypePontageIdx_ ), asnMission.type_pontage );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ExecuterVariantement::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ExecuterVariantement::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_executer_variantement );
    ASN1T_Mission_Pion_GEN_ExecuterVariantement& asnMission = *asnMsg.mission.u.mission_pion_gen_executer_variantement;

    NET_ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
