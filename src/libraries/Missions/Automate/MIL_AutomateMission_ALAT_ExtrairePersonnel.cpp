// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:55 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_ALAT_ExtrairePersonnel.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ALAT_ExtrairePersonnel::nDIAUnitesAHelitransporterIdx_ = 0 ;
int MIL_AutomateMission_ALAT_ExtrairePersonnel::nDIAPointDebarquementIdx_ = 0 ;
int MIL_AutomateMission_ALAT_ExtrairePersonnel::nDIAPointEmbarquementIdx_ = 0 ;
int MIL_AutomateMission_ALAT_ExtrairePersonnel::nDIAPointRegroupementIdx_ = 0 ;
int MIL_AutomateMission_ALAT_ExtrairePersonnel::nDIAPlotsRavitaillementIdx_ = 0 ;
int MIL_AutomateMission_ALAT_ExtrairePersonnel::nDIARavitaillementDebutMissionIdx_ = 0 ;
int MIL_AutomateMission_ALAT_ExtrairePersonnel::nDIAPorteeActionIdx_ = 0 ;
int MIL_AutomateMission_ALAT_ExtrairePersonnel::nDIAAmbianceMissionIdx_ = 0 ;
int MIL_AutomateMission_ALAT_ExtrairePersonnel::nDIAAvecMaterielIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_ExtrairePersonnel constructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_ALAT_ExtrairePersonnel::MIL_AutomateMission_ALAT_ExtrairePersonnel( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_ExtrairePersonnel destructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_ALAT_ExtrairePersonnel::~MIL_AutomateMission_ALAT_ExtrairePersonnel()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_ExtrairePersonnel::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ALAT_ExtrairePersonnel::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesAHelitransporterIdx_ = DEC_Tools::InitializeDIAField( "unitesAHelitransporter_", diaType );
    nDIAPointDebarquementIdx_ = DEC_Tools::InitializeDIAField( "pointDebarquement_", diaType );
    nDIAPointEmbarquementIdx_ = DEC_Tools::InitializeDIAField( "pointEmbarquement_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );
    nDIARavitaillementDebutMissionIdx_ = DEC_Tools::InitializeDIAField( "ravitaillementDebutMission_", diaType );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );
    nDIAAmbianceMissionIdx_ = DEC_Tools::InitializeDIAField( "ambianceMission_", diaType );
    nDIAAvecMaterielIdx_ = DEC_Tools::InitializeDIAField( "avecMateriel_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_ExtrairePersonnel::Initialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ALAT_ExtrairePersonnel::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ALAT_ExtrairePersonnel& asnMission = *asnMsg.mission.u.mission_automate_alat_extraire_personnel;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_helitransporter, GetVariable( nDIAUnitesAHelitransporterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_debarquement, pointDebarquement_, GetVariable( nDIAPointDebarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_embarquement, pointEmbarquement_, GetVariable( nDIAPointEmbarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), automate_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.ravitaillement_debut_mission, GetVariable( nDIARavitaillementDebutMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.portee_action, GetVariable( nDIAPorteeActionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.ambiance_mission, GetVariable( nDIAAmbianceMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.avec_materiel, GetVariable( nDIAAvecMaterielIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_ExtrairePersonnel::Terminate()
{
    NET_ASN_Tools::ResetBool( GetVariable( nDIAAvecMaterielIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_ExtrairePersonnel::Serialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_ExtrairePersonnel::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ALAT_ExtrairePersonnel& asnMission = *new ASN1T_Mission_Automate_ALAT_ExtrairePersonnel();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_alat_extraire_personnel;
    asnMsg.mission.u.mission_automate_alat_extraire_personnel  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesAHelitransporterIdx_ ), asnMission.unites_a_helitransporter );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDebarquementIdx_ ), asnMission.point_debarquement );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointEmbarquementIdx_ ), asnMission.point_embarquement );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, automate_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyBool( GetVariable( nDIARavitaillementDebutMissionIdx_ ), asnMission.ravitaillement_debut_mission );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPorteeActionIdx_ ), asnMission.portee_action );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ), asnMission.ambiance_mission );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAAvecMaterielIdx_ ), asnMission.avec_materiel );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_ExtrairePersonnel::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_ExtrairePersonnel::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_alat_extraire_personnel );
    ASN1T_Mission_Automate_ALAT_ExtrairePersonnel& asnMission = *asnMsg.mission.u.mission_automate_alat_extraire_personnel;

    NET_ASN_Tools::Delete( asnMission.unites_a_helitransporter );
    NET_ASN_Tools::Delete( asnMission.point_debarquement );
    NET_ASN_Tools::Delete( asnMission.point_embarquement );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
