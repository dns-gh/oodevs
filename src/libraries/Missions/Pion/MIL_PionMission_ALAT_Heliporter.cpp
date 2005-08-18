// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:43 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_ALAT_Heliporter.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ALAT_Heliporter::nDIAUnitesAHeliporterIdx_ = 0 ;
int MIL_PionMission_ALAT_Heliporter::nDIAPointDebarquementIdx_ = 0 ;
int MIL_PionMission_ALAT_Heliporter::nDIAPointEmbarquementIdx_ = 0 ;
int MIL_PionMission_ALAT_Heliporter::nDIAPointRegroupementIdx_ = 0 ;
int MIL_PionMission_ALAT_Heliporter::nDIAPlotsRavitaillementIdx_ = 0 ;
int MIL_PionMission_ALAT_Heliporter::nDIARavitaillementDebutMissionIdx_ = 0 ;
int MIL_PionMission_ALAT_Heliporter::nDIAPorteeActionIdx_ = 0 ;
int MIL_PionMission_ALAT_Heliporter::nDIAAttendreUniteIdx_ = 0 ;
int MIL_PionMission_ALAT_Heliporter::nDIAAvecMaterielIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Heliporter::InitializeDIA
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ALAT_Heliporter::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesAHeliporterIdx_ = DEC_Tools::InitializeDIAField( "unitesAHeliporter_", diaType );
    nDIAPointDebarquementIdx_ = DEC_Tools::InitializeDIAField( "pointDebarquement_", diaType );
    nDIAPointEmbarquementIdx_ = DEC_Tools::InitializeDIAField( "pointEmbarquement_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );
    nDIARavitaillementDebutMissionIdx_ = DEC_Tools::InitializeDIAField( "ravitaillementDebutMission_", diaType );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );
    nDIAAttendreUniteIdx_ = DEC_Tools::InitializeDIAField( "attendreUnite_", diaType );
    nDIAAvecMaterielIdx_ = DEC_Tools::InitializeDIAField( "avecMateriel_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Heliporter constructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_Heliporter::MIL_PionMission_ALAT_Heliporter( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Heliporter destructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_Heliporter::~MIL_PionMission_ALAT_Heliporter()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Heliporter::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ALAT_Heliporter::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ALAT_Heliporter& asnMission = *asnMsg.mission.u.mission_pion_alat_heliporter;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_heliporter, GetVariable( nDIAUnitesAHeliporterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_debarquement, pointDebarquement_, GetVariable( nDIAPointDebarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_embarquement, pointEmbarquement_, GetVariable( nDIAPointEmbarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.ravitaillement_debut_mission, GetVariable( nDIARavitaillementDebutMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.portee_action, GetVariable( nDIAPorteeActionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.attendre_unite, GetVariable( nDIAAttendreUniteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.avec_materiel, GetVariable( nDIAAvecMaterielIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Heliporter::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_Heliporter::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAgentList( GetVariable( nDIAUnitesAHeliporterIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointDebarquement_, GetVariable( nDIAPointDebarquementIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointEmbarquement_, GetVariable( nDIAPointEmbarquementIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::ResetObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIARavitaillementDebutMissionIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAPorteeActionIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIAAttendreUniteIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIAAvecMaterielIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Heliporter::Initialize
// Created: 2005-08-17 - 16:30:43
// -----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_Heliporter::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ALAT_Heliporter& mission = static_cast< MIL_PionMission_ALAT_Heliporter& >( missionTmp );

    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAUnitesAHeliporterIdx_ ), GetVariable( nDIAUnitesAHeliporterIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDebarquementIdx_ ), pointDebarquement_, GetVariable( nDIAPointDebarquementIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointEmbarquementIdx_ ), pointEmbarquement_, GetVariable( nDIAPointEmbarquementIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRegroupementIdx_ ), pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::CopyObjectKnowledgeList( mission.GetVariable( nDIAPlotsRavitaillementIdx_ ), GetVariable( nDIAPlotsRavitaillementIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIARavitaillementDebutMissionIdx_ ), GetVariable( nDIARavitaillementDebutMissionIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAPorteeActionIdx_ ), GetVariable( nDIAPorteeActionIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIAAttendreUniteIdx_ ), GetVariable( nDIAAttendreUniteIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIAAvecMaterielIdx_ ), GetVariable( nDIAAvecMaterielIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Heliporter::Terminate
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_Heliporter::Terminate()
{
    NET_ASN_Tools::ResetBool( GetVariable( nDIAAvecMaterielIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Heliporter::Serialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_Heliporter::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ALAT_Heliporter& asnMission = *new ASN1T_Mission_Pion_ALAT_Heliporter();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_alat_heliporter;
    asnMsg.mission.u.mission_pion_alat_heliporter  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesAHeliporterIdx_ ), asnMission.unites_a_heliporter );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDebarquementIdx_ ), asnMission.point_debarquement );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointEmbarquementIdx_ ), asnMission.point_embarquement );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, pion_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyBool( GetVariable( nDIARavitaillementDebutMissionIdx_ ), asnMission.ravitaillement_debut_mission );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPorteeActionIdx_ ), asnMission.portee_action );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAAttendreUniteIdx_ ), asnMission.attendre_unite );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAAvecMaterielIdx_ ), asnMission.avec_materiel );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Heliporter::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_Heliporter::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_alat_heliporter );
    ASN1T_Mission_Pion_ALAT_Heliporter& asnMission = *asnMsg.mission.u.mission_pion_alat_heliporter;

    NET_ASN_Tools::Delete( asnMission.unites_a_heliporter );
    NET_ASN_Tools::Delete( asnMission.point_debarquement );
    NET_ASN_Tools::Delete( asnMission.point_embarquement );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
