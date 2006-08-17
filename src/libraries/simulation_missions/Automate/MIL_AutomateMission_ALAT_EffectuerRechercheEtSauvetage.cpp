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

#include "MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::nDIAUnitesASecourirIdx_ = 0 ;
int MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::nDIAPointRegroupementIdx_ = 0 ;
int MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::nDIAPlotsRavitaillementIdx_ = 0 ;
int MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::nDIARavitaillementDebutMissionIdx_ = 0 ;
int MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::nDIAPorteeActionIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::~MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesASecourirIdx_ = DEC_Tools::InitializeDIAField( "unitesASecourir_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );
    nDIARavitaillementDebutMissionIdx_ = DEC_Tools::InitializeDIAField( "ravitaillementDebutMission_", diaType );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage& asnMission = *asnMsg.mission.u.mission_automate_alat_effectuer_recherche_et_sauvetage;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_secourir, GetVariable( nDIAUnitesASecourirIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), automate_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.ravitaillement_debut_mission, GetVariable( nDIARavitaillementDebutMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.portee_action, GetVariable( nDIAPorteeActionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage& asnMission = *new ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_alat_effectuer_recherche_et_sauvetage;
    asnMsg.mission.u.mission_automate_alat_effectuer_recherche_et_sauvetage  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesASecourirIdx_ ), asnMission.unites_a_secourir );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, automate_.GetKnowledgeGroup() );
    NET_ASN_Tools::CopyBool( GetVariable( nDIARavitaillementDebutMissionIdx_ ), asnMission.ravitaillement_debut_mission );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPorteeActionIdx_ ), asnMission.portee_action );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_alat_effectuer_recherche_et_sauvetage );
    ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage& asnMission = *asnMsg.mission.u.mission_automate_alat_effectuer_recherche_et_sauvetage;

    NET_ASN_Tools::Delete( asnMission.unites_a_secourir );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
