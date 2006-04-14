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
#include "MIL_PionMission_ALAT_EvacuationSanitaire.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ALAT_EvacuationSanitaire::nDIAPointDebarquementIdx_ = 0 ;
int MIL_PionMission_ALAT_EvacuationSanitaire::nDIAZoneExtractionIdx_ = 0 ;
int MIL_PionMission_ALAT_EvacuationSanitaire::nDIAPointRegroupementIdx_ = 0 ;
int MIL_PionMission_ALAT_EvacuationSanitaire::nDIAPlotsRavitaillementIdx_ = 0 ;
int MIL_PionMission_ALAT_EvacuationSanitaire::nDIARavitaillementDebutMissionIdx_ = 0 ;
int MIL_PionMission_ALAT_EvacuationSanitaire::nDIAPorteeActionIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EvacuationSanitaire::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ALAT_EvacuationSanitaire::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointDebarquementIdx_ = DEC_Tools::InitializeDIAField( "pointDebarquement_", diaType );
    nDIAZoneExtractionIdx_ = DEC_Tools::InitializeDIAField( "zoneExtraction_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );
    nDIARavitaillementDebutMissionIdx_ = DEC_Tools::InitializeDIAField( "ravitaillementDebutMission_", diaType );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EvacuationSanitaire constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_EvacuationSanitaire::MIL_PionMission_ALAT_EvacuationSanitaire( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EvacuationSanitaire destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_EvacuationSanitaire::~MIL_PionMission_ALAT_EvacuationSanitaire()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EvacuationSanitaire::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ALAT_EvacuationSanitaire::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ALAT_EvacuationSanitaire& asnMission = *asnMsg.mission.u.mission_pion_alat_evacuation_sanitaire;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_debarquement, GetVariable( nDIAPointDebarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_extraction, GetVariable( nDIAZoneExtractionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.ravitaillement_debut_mission, GetVariable( nDIARavitaillementDebutMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.portee_action, GetVariable( nDIAPorteeActionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EvacuationSanitaire::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_EvacuationSanitaire::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

                        
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EvacuationSanitaire::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_EvacuationSanitaire::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ALAT_EvacuationSanitaire& mission = static_cast< MIL_PionMission_ALAT_EvacuationSanitaire& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDebarquementIdx_ ), GetVariable( nDIAPointDebarquementIdx_ ) );
    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIAZoneExtractionIdx_ ), GetVariable( nDIAZoneExtractionIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRegroupementIdx_ ), GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::CopyObjectKnowledgeList( mission.GetVariable( nDIAPlotsRavitaillementIdx_ ), GetVariable( nDIAPlotsRavitaillementIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIARavitaillementDebutMissionIdx_ ), GetVariable( nDIARavitaillementDebutMissionIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAPorteeActionIdx_ ), GetVariable( nDIAPorteeActionIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EvacuationSanitaire::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_EvacuationSanitaire::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EvacuationSanitaire::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_EvacuationSanitaire::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ALAT_EvacuationSanitaire& asnMission = *new ASN1T_Mission_Pion_ALAT_EvacuationSanitaire();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_alat_evacuation_sanitaire;
    asnMsg.mission.u.mission_pion_alat_evacuation_sanitaire  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDebarquementIdx_ ), asnMission.point_debarquement );
    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneExtractionIdx_ ), asnMission.zone_extraction );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, pion_.GetKnowledgeGroup() );
    NET_ASN_Tools::CopyBool( GetVariable( nDIARavitaillementDebutMissionIdx_ ), asnMission.ravitaillement_debut_mission );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPorteeActionIdx_ ), asnMission.portee_action );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_EvacuationSanitaire::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_EvacuationSanitaire::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_alat_evacuation_sanitaire );
    ASN1T_Mission_Pion_ALAT_EvacuationSanitaire& asnMission = *asnMsg.mission.u.mission_pion_alat_evacuation_sanitaire;

    NET_ASN_Tools::Delete( asnMission.point_debarquement );
    NET_ASN_Tools::Delete( asnMission.zone_extraction );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
