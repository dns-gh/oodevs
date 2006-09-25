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

#include "simulation_missions_pch.h"
#include "MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::nDIAUnitesASecourirIdx_ = 0 ;
int MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::nDIAPointRegroupementIdx_ = 0 ;
int MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::nDIAPlotsRavitaillementIdx_ = 0 ;
int MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::nDIAPorteeActionIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesASecourirIdx_ = DEC_Tools::InitializeDIAField( "unitesASecourir_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::~MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage& asnMission = *asnMsg.mission.u.mission_pion_alat_effectuer_recherche_et_sauvetage;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_secourir, GetVariable( nDIAUnitesASecourirIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.portee_action, GetVariable( nDIAPorteeActionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

                
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage& mission = static_cast< MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage& >( missionTmp );

    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAUnitesASecourirIdx_ ), GetVariable( nDIAUnitesASecourirIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRegroupementIdx_ ), GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::CopyObjectKnowledgeList( mission.GetVariable( nDIAPlotsRavitaillementIdx_ ), GetVariable( nDIAPlotsRavitaillementIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAPorteeActionIdx_ ), GetVariable( nDIAPorteeActionIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage& asnMission = *new ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_alat_effectuer_recherche_et_sauvetage;
    asnMsg.mission.u.mission_pion_alat_effectuer_recherche_et_sauvetage  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesASecourirIdx_ ), asnMission.unites_a_secourir );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, pion_.GetKnowledgeGroup() );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPorteeActionIdx_ ), asnMission.portee_action );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_alat_effectuer_recherche_et_sauvetage );
    ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage& asnMission = *asnMsg.mission.u.mission_pion_alat_effectuer_recherche_et_sauvetage;

    NET_ASN_Tools::Delete( asnMission.unites_a_secourir );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
