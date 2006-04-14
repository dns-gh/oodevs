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
#include "MIL_PionMission_ALAT_Escorter.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ALAT_Escorter::nDIAUnitesAEscorterIdx_ = 0 ;
int MIL_PionMission_ALAT_Escorter::nDIAPointRegroupementIdx_ = 0 ;
int MIL_PionMission_ALAT_Escorter::nDIACiblesPrioritairesIdx_ = 0 ;
int MIL_PionMission_ALAT_Escorter::nDIAPlotsRavitaillementIdx_ = 0 ;
int MIL_PionMission_ALAT_Escorter::nDIARavitaillementDebutMissionIdx_ = 0 ;
int MIL_PionMission_ALAT_Escorter::nDIAPorteeActionIdx_ = 0 ;
int MIL_PionMission_ALAT_Escorter::nDIAAmbianceMissionIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Escorter::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ALAT_Escorter::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesAEscorterIdx_ = DEC_Tools::InitializeDIAField( "unitesAEscorter_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIACiblesPrioritairesIdx_ = DEC_Tools::InitializeDIAField( "ciblesPrioritaires_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );
    nDIARavitaillementDebutMissionIdx_ = DEC_Tools::InitializeDIAField( "ravitaillementDebutMission_", diaType );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );
    nDIAAmbianceMissionIdx_ = DEC_Tools::InitializeDIAField( "ambianceMission_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Escorter constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_Escorter::MIL_PionMission_ALAT_Escorter( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Escorter destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_Escorter::~MIL_PionMission_ALAT_Escorter()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Escorter::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ALAT_Escorter::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ALAT_Escorter& asnMission = *asnMsg.mission.u.mission_pion_alat_escorter;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_escorter, GetVariable( nDIAUnitesAEscorterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNatureAtlas( asnMission.cibles_prioritaires, GetVariable( nDIACiblesPrioritairesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.ravitaillement_debut_mission, GetVariable( nDIARavitaillementDebutMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.portee_action, GetVariable( nDIAPorteeActionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.ambiance_mission, GetVariable( nDIAAmbianceMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Escorter::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_Escorter::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

                            
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Escorter::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_Escorter::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ALAT_Escorter& mission = static_cast< MIL_PionMission_ALAT_Escorter& >( missionTmp );

    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAUnitesAEscorterIdx_ ), GetVariable( nDIAUnitesAEscorterIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRegroupementIdx_ ), GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::CopyNatureAtlas( mission.GetVariable( nDIACiblesPrioritairesIdx_ ), GetVariable( nDIACiblesPrioritairesIdx_ ) );
    NET_ASN_Tools::CopyObjectKnowledgeList( mission.GetVariable( nDIAPlotsRavitaillementIdx_ ), GetVariable( nDIAPlotsRavitaillementIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIARavitaillementDebutMissionIdx_ ), GetVariable( nDIARavitaillementDebutMissionIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAPorteeActionIdx_ ), GetVariable( nDIAPorteeActionIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAAmbianceMissionIdx_ ), GetVariable( nDIAAmbianceMissionIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Escorter::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_Escorter::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Escorter::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_Escorter::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ALAT_Escorter& asnMission = *new ASN1T_Mission_Pion_ALAT_Escorter();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_alat_escorter;
    asnMsg.mission.u.mission_pion_alat_escorter  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesAEscorterIdx_ ), asnMission.unites_a_escorter );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyNatureAtlas( GetVariable( nDIACiblesPrioritairesIdx_ ), asnMission.cibles_prioritaires );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, pion_.GetKnowledgeGroup() );
    NET_ASN_Tools::CopyBool( GetVariable( nDIARavitaillementDebutMissionIdx_ ), asnMission.ravitaillement_debut_mission );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPorteeActionIdx_ ), asnMission.portee_action );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ), asnMission.ambiance_mission );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Escorter::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_Escorter::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_alat_escorter );
    ASN1T_Mission_Pion_ALAT_Escorter& asnMission = *asnMsg.mission.u.mission_pion_alat_escorter;

    NET_ASN_Tools::Delete( asnMission.unites_a_escorter );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
