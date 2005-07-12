// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:2 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_ALAT_Attaquer.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ALAT_Attaquer::nDIACiblesPrioritairesIdx_ = 0 ;
int MIL_PionMission_ALAT_Attaquer::nDIAPointRegroupementIdx_ = 0 ;
int MIL_PionMission_ALAT_Attaquer::nDIAPlotsRavitaillementIdx_ = 0 ;
int MIL_PionMission_ALAT_Attaquer::nDIAPorteeActionIdx_ = 0 ;
int MIL_PionMission_ALAT_Attaquer::nDIAAmbianceMissionIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Attaquer::InitializeDIA
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ALAT_Attaquer::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIACiblesPrioritairesIdx_ = DEC_Tools::InitializeDIAField( "ciblesPrioritaires_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );
    nDIAAmbianceMissionIdx_ = DEC_Tools::InitializeDIAField( "ambianceMission_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Attaquer constructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_Attaquer::MIL_PionMission_ALAT_Attaquer( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Attaquer destructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_Attaquer::~MIL_PionMission_ALAT_Attaquer()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Attaquer::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ALAT_Attaquer::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ALAT_Attaquer& asnMission = *asnMsg.mission.u.mission_pion_alat_attaquer;
    if( !NET_ASN_Tools::CopyNatureAtlas( asnMission.cibles_prioritaires, GetVariable( nDIACiblesPrioritairesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.portee_action, GetVariable( nDIAPorteeActionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.ambiance_mission, GetVariable( nDIAAmbianceMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Attaquer::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_Attaquer::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetNatureAtlas( GetVariable( nDIACiblesPrioritairesIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::ResetObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAPorteeActionIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Attaquer::Initialize
// Created: 2005-6-28 - 14:3:2
// -----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_Attaquer::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ALAT_Attaquer& mission = static_cast< MIL_PionMission_ALAT_Attaquer& >( missionTmp );

    NET_ASN_Tools::CopyNatureAtlas( mission.GetVariable( nDIACiblesPrioritairesIdx_ ), GetVariable( nDIACiblesPrioritairesIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRegroupementIdx_ ), pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::CopyObjectKnowledgeList( mission.GetVariable( nDIAPlotsRavitaillementIdx_ ), GetVariable( nDIAPlotsRavitaillementIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAPorteeActionIdx_ ), GetVariable( nDIAPorteeActionIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAAmbianceMissionIdx_ ), GetVariable( nDIAAmbianceMissionIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Attaquer::Terminate
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_Attaquer::Terminate()
{
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Attaquer::Serialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_Attaquer::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ALAT_Attaquer& asnMission = *new ASN1T_Mission_Pion_ALAT_Attaquer();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_alat_attaquer;
    asnMsg.mission.u.mission_pion_alat_attaquer  = &asnMission;

    NET_ASN_Tools::CopyNatureAtlas( GetVariable( nDIACiblesPrioritairesIdx_ ), asnMission.cibles_prioritaires );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, pion_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPorteeActionIdx_ ), asnMission.portee_action );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ), asnMission.ambiance_mission );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_Attaquer::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_Attaquer::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_alat_attaquer );
    ASN1T_Mission_Pion_ALAT_Attaquer& asnMission = *asnMsg.mission.u.mission_pion_alat_attaquer;

    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
