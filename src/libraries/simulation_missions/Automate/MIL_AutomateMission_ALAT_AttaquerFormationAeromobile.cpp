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

#include "MIL_AutomateMission_ALAT_AttaquerFormationAeromobile.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::nDIAUnitesAAttaquerIdx_ = 0 ;
int MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::nDIAPointRegroupementIdx_ = 0 ;
int MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::nDIAPlotsRavitaillementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_AttaquerFormationAeromobile constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::MIL_AutomateMission_ALAT_AttaquerFormationAeromobile( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_AttaquerFormationAeromobile destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::~MIL_AutomateMission_ALAT_AttaquerFormationAeromobile()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesAAttaquerIdx_ = DEC_Tools::InitializeDIAField( "unitesAAttaquer_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile& asnMission = *asnMsg.mission.u.mission_automate_alat_attaquer_formation_aeromobile;
    if( !NET_ASN_Tools::CopyAgentKnowledgeList( asnMission.unites_a_attaquer, GetVariable( nDIAUnitesAAttaquerIdx_ ), automate_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), automate_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile& asnMission = *new ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_alat_attaquer_formation_aeromobile;
    asnMsg.mission.u.mission_automate_alat_attaquer_formation_aeromobile  = &asnMission;

    NET_ASN_Tools::CopyAgentKnowledgeList( GetVariable( nDIAUnitesAAttaquerIdx_ ), asnMission.unites_a_attaquer, automate_.GetKnowledgeGroup() );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, automate_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_AttaquerFormationAeromobile::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_alat_attaquer_formation_aeromobile );
    ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile& asnMission = *asnMsg.mission.u.mission_automate_alat_attaquer_formation_aeromobile;

    NET_ASN_Tools::Delete( asnMission.unites_a_attaquer );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
