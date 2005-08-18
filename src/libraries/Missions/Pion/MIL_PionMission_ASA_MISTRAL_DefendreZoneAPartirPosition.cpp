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
#include "MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::nDIAZoneIdx_ = 0 ;
int MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::nDIAPointDeDeploiementIdx_ = 0 ;
int MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::nDIAModeDeploiementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::InitializeDIA
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );
    nDIAPointDeDeploiementIdx_ = DEC_Tools::InitializeDIAField( "pointDeDeploiement_", diaType );
    nDIAModeDeploiementIdx_ = DEC_Tools::InitializeDIAField( "modeDeploiement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition constructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition destructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::~MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition& asnMission = *asnMsg.mission.u.mission_pion_asa_mistral_defendre_zone_a_partir_position;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, zone_, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_de_deploiement, pointDeDeploiement_, GetVariable( nDIAPointDeDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.mode_deploiement, GetVariable( nDIAModeDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPolygon( zone_, GetVariable( nDIAZoneIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointDeDeploiement_, GetVariable( nDIAPointDeDeploiementIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAModeDeploiementIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::Initialize
// Created: 2005-08-17 - 16:30:43
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition& mission = static_cast< MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIAZoneIdx_ ), zone_, GetVariable( nDIAZoneIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDeDeploiementIdx_ ), pointDeDeploiement_, GetVariable( nDIAPointDeDeploiementIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAModeDeploiementIdx_ ), GetVariable( nDIAModeDeploiementIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::Terminate
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::Terminate()
{
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAModeDeploiementIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::Serialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_asa_mistral_defendre_zone_a_partir_position;
    asnMsg.mission.u.mission_pion_asa_mistral_defendre_zone_a_partir_position  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDeDeploiementIdx_ ), asnMission.point_de_deploiement );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAModeDeploiementIdx_ ), asnMission.mode_deploiement );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_asa_mistral_defendre_zone_a_partir_position );
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition& asnMission = *asnMsg.mission.u.mission_pion_asa_mistral_defendre_zone_a_partir_position;

    NET_ASN_Tools::Delete( asnMission.zone );
    NET_ASN_Tools::Delete( asnMission.point_de_deploiement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
