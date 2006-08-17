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
#include "MIL_PionMission_ALAT_DetruireNeutraliserDansZone.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ALAT_DetruireNeutraliserDansZone::nDIAZoneIdx_ = 0 ;
int MIL_PionMission_ALAT_DetruireNeutraliserDansZone::nDIAPointDislocationIdx_ = 0 ;
int MIL_PionMission_ALAT_DetruireNeutraliserDansZone::nDIACiblesPrioritairesIdx_ = 0 ;
int MIL_PionMission_ALAT_DetruireNeutraliserDansZone::nDIAPointRegroupementIdx_ = 0 ;
int MIL_PionMission_ALAT_DetruireNeutraliserDansZone::nDIAPlotsRavitaillementIdx_ = 0 ;
int MIL_PionMission_ALAT_DetruireNeutraliserDansZone::nDIARavitaillementDebutMissionIdx_ = 0 ;
int MIL_PionMission_ALAT_DetruireNeutraliserDansZone::nDIAPorteeActionIdx_ = 0 ;
int MIL_PionMission_ALAT_DetruireNeutraliserDansZone::nDIAAmbianceMissionIdx_ = 0 ;
int MIL_PionMission_ALAT_DetruireNeutraliserDansZone::nDIANeutraliserIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_DetruireNeutraliserDansZone::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ALAT_DetruireNeutraliserDansZone::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );
    nDIAPointDislocationIdx_ = DEC_Tools::InitializeDIAField( "pointDislocation_", diaType );
    nDIACiblesPrioritairesIdx_ = DEC_Tools::InitializeDIAField( "ciblesPrioritaires_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );
    nDIARavitaillementDebutMissionIdx_ = DEC_Tools::InitializeDIAField( "ravitaillementDebutMission_", diaType );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );
    nDIAAmbianceMissionIdx_ = DEC_Tools::InitializeDIAField( "ambianceMission_", diaType );
    nDIANeutraliserIdx_ = DEC_Tools::InitializeDIAField( "neutraliser_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_DetruireNeutraliserDansZone constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_DetruireNeutraliserDansZone::MIL_PionMission_ALAT_DetruireNeutraliserDansZone( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_DetruireNeutraliserDansZone destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_DetruireNeutraliserDansZone::~MIL_PionMission_ALAT_DetruireNeutraliserDansZone()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_DetruireNeutraliserDansZone::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ALAT_DetruireNeutraliserDansZone::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone& asnMission = *asnMsg.mission.u.mission_pion_alat_detruire_neutraliser_dans_zone;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_dislocation, GetVariable( nDIAPointDislocationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNatureAtlas( asnMission.cibles_prioritaires, GetVariable( nDIACiblesPrioritairesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.ravitaillement_debut_mission, GetVariable( nDIARavitaillementDebutMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.portee_action, GetVariable( nDIAPorteeActionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.ambiance_mission, GetVariable( nDIAAmbianceMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.neutraliser, GetVariable( nDIANeutraliserIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_DetruireNeutraliserDansZone::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_DetruireNeutraliserDansZone::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

                                    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_DetruireNeutraliserDansZone::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_DetruireNeutraliserDansZone::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ALAT_DetruireNeutraliserDansZone& mission = static_cast< MIL_PionMission_ALAT_DetruireNeutraliserDansZone& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIAZoneIdx_ ), GetVariable( nDIAZoneIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDislocationIdx_ ), GetVariable( nDIAPointDislocationIdx_ ) );
    NET_ASN_Tools::CopyNatureAtlas( mission.GetVariable( nDIACiblesPrioritairesIdx_ ), GetVariable( nDIACiblesPrioritairesIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRegroupementIdx_ ), GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::CopyObjectKnowledgeList( mission.GetVariable( nDIAPlotsRavitaillementIdx_ ), GetVariable( nDIAPlotsRavitaillementIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIARavitaillementDebutMissionIdx_ ), GetVariable( nDIARavitaillementDebutMissionIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAPorteeActionIdx_ ), GetVariable( nDIAPorteeActionIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAAmbianceMissionIdx_ ), GetVariable( nDIAAmbianceMissionIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIANeutraliserIdx_ ), GetVariable( nDIANeutraliserIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_DetruireNeutraliserDansZone::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_DetruireNeutraliserDansZone::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_DetruireNeutraliserDansZone::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_DetruireNeutraliserDansZone::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone& asnMission = *new ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_alat_detruire_neutraliser_dans_zone;
    asnMsg.mission.u.mission_pion_alat_detruire_neutraliser_dans_zone  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDislocationIdx_ ), asnMission.point_dislocation );
    NET_ASN_Tools::CopyNatureAtlas( GetVariable( nDIACiblesPrioritairesIdx_ ), asnMission.cibles_prioritaires );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, pion_.GetKnowledgeGroup() );
    NET_ASN_Tools::CopyBool( GetVariable( nDIARavitaillementDebutMissionIdx_ ), asnMission.ravitaillement_debut_mission );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPorteeActionIdx_ ), asnMission.portee_action );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ), asnMission.ambiance_mission );
    NET_ASN_Tools::CopyBool( GetVariable( nDIANeutraliserIdx_ ), asnMission.neutraliser );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_DetruireNeutraliserDansZone::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_DetruireNeutraliserDansZone::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_alat_detruire_neutraliser_dans_zone );
    ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone& asnMission = *asnMsg.mission.u.mission_pion_alat_detruire_neutraliser_dans_zone;

    NET_ASN_Tools::Delete( asnMission.zone );
    NET_ASN_Tools::Delete( asnMission.point_dislocation );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
