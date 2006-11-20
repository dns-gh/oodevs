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

#include "simulation_missions_pch.h"
#include "MIL_PionMission_ASA_DefendreSiteAPartirPosition.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_ASA_DefendreSiteAPartirPosition::nDIASiteIdx_ = 0 ;
int MIL_PionMission_ASA_DefendreSiteAPartirPosition::nDIAPointDeDeploiementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSiteAPartirPosition::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASA_DefendreSiteAPartirPosition::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASiteIdx_ = DEC_Tools::InitializeDIAField( "site_", diaType );
    nDIAPointDeDeploiementIdx_ = DEC_Tools::InitializeDIAField( "pointDeDeploiement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSiteAPartirPosition constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_DefendreSiteAPartirPosition::MIL_PionMission_ASA_DefendreSiteAPartirPosition( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSiteAPartirPosition destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_DefendreSiteAPartirPosition::~MIL_PionMission_ASA_DefendreSiteAPartirPosition()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSiteAPartirPosition::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASA_DefendreSiteAPartirPosition::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition& asnMission = *asnMsg.mission.u.mission_pion_asa_defendre_site_a_partir_position;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.site, GetVariable( nDIASiteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_de_deploiement, GetVariable( nDIAPointDeDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSiteAPartirPosition::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASA_DefendreSiteAPartirPosition::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSiteAPartirPosition::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASA_DefendreSiteAPartirPosition::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASA_DefendreSiteAPartirPosition& mission = static_cast< MIL_PionMission_ASA_DefendreSiteAPartirPosition& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIASiteIdx_ ), GetVariable( nDIASiteIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDeDeploiementIdx_ ), GetVariable( nDIAPointDeDeploiementIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSiteAPartirPosition::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_DefendreSiteAPartirPosition::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_asa_defendre_site_a_partir_position;
    asnMsg.mission.u.mission_pion_asa_defendre_site_a_partir_position  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIASiteIdx_ ), asnMission.site );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDeDeploiementIdx_ ), asnMission.point_de_deploiement );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSiteAPartirPosition::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_DefendreSiteAPartirPosition::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_asa_defendre_site_a_partir_position );
    ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition& asnMission = *asnMsg.mission.u.mission_pion_asa_defendre_site_a_partir_position;

    NET_ASN_Tools::Delete( asnMission.site );
    NET_ASN_Tools::Delete( asnMission.point_de_deploiement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
