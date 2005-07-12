// *****************************************************************************
//
// $Created: 2005-4-27 - 17:37:45 $
// $Archive: /MVW_v10/Build/SDK/Missions/src/Pion/MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 17:37 $
// $Revision: 16 $
// $Workfile: MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition.h"

#include "Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "Entities/Orders/Pion/MIL_PionMissionType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

int MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::nDIAZoneIdx_ = 0 ;
int MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::nDIAPointDeDeploiementIdx_ = 0 ;
int MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::nDIAModeDeploiementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::InitializeDIA
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );
    nDIAPointDeDeploiementIdx_ = DEC_Tools::InitializeDIAField( "pointDeDeploiement_", diaType );
    nDIAModeDeploiementIdx_ = DEC_Tools::InitializeDIAField( "modeDeploiement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition constructor
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition destructor
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::~MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::Initialize
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASA_MISTRAL_SurveillerAPartirPosition& asnMission = *asnMsg.mission.u.mission_pion_asa_mistral_surveiller_a_partir_position;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, zone_, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_de_deploiement, pointDeDeploiement_, GetVariable( nDIAPointDeDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.mode_deploiement, GetVariable( nDIAModeDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::Initialize
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPolygon( zone_, GetVariable( nDIAZoneIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointDeDeploiement_, GetVariable( nDIAPointDeDeploiementIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAModeDeploiementIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::Initialize
// Created: 2005-4-27 - 17:37:45
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition& mission = static_cast< MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIAZoneIdx_ ), zone_, GetVariable( nDIAZoneIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDeDeploiementIdx_ ), pointDeDeploiement_, GetVariable( nDIAPointDeDeploiementIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAModeDeploiementIdx_ ), GetVariable( nDIAModeDeploiementIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::Terminate
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::Terminate()
{
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAModeDeploiementIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::Serialize
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASA_MISTRAL_SurveillerAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_SurveillerAPartirPosition();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_asa_mistral_surveiller_a_partir_position;
    asnMsg.mission.u.mission_pion_asa_mistral_surveiller_a_partir_position  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDeDeploiementIdx_ ), asnMission.point_de_deploiement );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAModeDeploiementIdx_ ), asnMission.mode_deploiement );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::CleanAfterSerialization
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_asa_mistral_surveiller_a_partir_position );
    ASN1T_Mission_Pion_ASA_MISTRAL_SurveillerAPartirPosition& asnMission = *asnMsg.mission.u.mission_pion_asa_mistral_surveiller_a_partir_position;

    NET_ASN_Tools::Delete( asnMission.zone );
    NET_ASN_Tools::Delete( asnMission.point_de_deploiement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
