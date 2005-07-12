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
#include "MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::nDIAPointRegroupementIdx_ = 0 ;
int MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::nDIAPlotsRavitaillementIdx_ = 0 ;
int MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::nDIAPorteeActionIdx_ = 0 ;
int MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::nDIAAmbianceMissionIdx_ = 0 ;
int MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::nDIAPointDislocationIdx_ = 0 ;
int MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::nDIAZoneIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::InitializeDIA
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );
    nDIAAmbianceMissionIdx_ = DEC_Tools::InitializeDIAField( "ambianceMission_", diaType );
    nDIAPointDislocationIdx_ = DEC_Tools::InitializeDIAField( "pointDislocation_", diaType );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur constructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur destructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::~MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur& asnMission = *asnMsg.mission.u.mission_pion_alat_reconnaitre_dans_la_profondeur;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.portee_action, GetVariable( nDIAPorteeActionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.ambiance_mission, GetVariable( nDIAAmbianceMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_dislocation, pointDislocation_, GetVariable( nDIAPointDislocationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyLocation( asnMission.zone, zone_, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::ResetObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAPorteeActionIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointDislocation_, GetVariable( nDIAPointDislocationIdx_ ) );
    NET_ASN_Tools::ResetLocation( zone_, GetVariable( nDIAZoneIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::Initialize
// Created: 2005-6-28 - 14:3:2
// -----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur& mission = static_cast< MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRegroupementIdx_ ), pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::CopyObjectKnowledgeList( mission.GetVariable( nDIAPlotsRavitaillementIdx_ ), GetVariable( nDIAPlotsRavitaillementIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAPorteeActionIdx_ ), GetVariable( nDIAPorteeActionIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAAmbianceMissionIdx_ ), GetVariable( nDIAAmbianceMissionIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDislocationIdx_ ), pointDislocation_, GetVariable( nDIAPointDislocationIdx_ ) );
    NET_ASN_Tools::CopyLocation( mission.GetVariable( nDIAZoneIdx_ ), zone_, GetVariable( nDIAZoneIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::Terminate
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::Terminate()
{
    NET_ASN_Tools::ResetLocation( zone_, GetVariable( nDIAZoneIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::Serialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur& asnMission = *new ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_alat_reconnaitre_dans_la_profondeur;
    asnMsg.mission.u.mission_pion_alat_reconnaitre_dans_la_profondeur  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, pion_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPorteeActionIdx_ ), asnMission.portee_action );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ), asnMission.ambiance_mission );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDislocationIdx_ ), asnMission.point_dislocation );
    NET_ASN_Tools::CopyLocation( GetVariable( nDIAZoneIdx_ ), asnMission.zone );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_alat_reconnaitre_dans_la_profondeur );
    ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur& asnMission = *asnMsg.mission.u.mission_pion_alat_reconnaitre_dans_la_profondeur;

    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );
    NET_ASN_Tools::Delete( asnMission.point_dislocation );
    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
