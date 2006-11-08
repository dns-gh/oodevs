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
#include "MIL_PionMission_ASA_MISTRAL_DefendreSite.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_ASA_MISTRAL_DefendreSite::nDIASiteIdx_ = 0 ;
int MIL_PionMission_ASA_MISTRAL_DefendreSite::nDIAModeDeploiementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreSite::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASA_MISTRAL_DefendreSite::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASiteIdx_ = DEC_Tools::InitializeDIAField( "site_", diaType );
    nDIAModeDeploiementIdx_ = DEC_Tools::InitializeDIAField( "modeDeploiement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreSite constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_MISTRAL_DefendreSite::MIL_PionMission_ASA_MISTRAL_DefendreSite( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreSite destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_MISTRAL_DefendreSite::~MIL_PionMission_ASA_MISTRAL_DefendreSite()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreSite::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASA_MISTRAL_DefendreSite::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite& asnMission = *asnMsg.mission.u.mission_pion_asa_mistral_defendre_site;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.site, GetVariable( nDIASiteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.mode_deploiement, GetVariable( nDIAModeDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreSite::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASA_MISTRAL_DefendreSite::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreSite::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASA_MISTRAL_DefendreSite::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASA_MISTRAL_DefendreSite& mission = static_cast< MIL_PionMission_ASA_MISTRAL_DefendreSite& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIASiteIdx_ ), GetVariable( nDIASiteIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAModeDeploiementIdx_ ), GetVariable( nDIAModeDeploiementIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreSite::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_MISTRAL_DefendreSite::Terminate()
{
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreSite::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_MISTRAL_DefendreSite::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_asa_mistral_defendre_site;
    asnMsg.mission.u.mission_pion_asa_mistral_defendre_site  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIASiteIdx_ ), asnMission.site );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAModeDeploiementIdx_ ), asnMission.mode_deploiement );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_MISTRAL_DefendreSite::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_MISTRAL_DefendreSite::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_asa_mistral_defendre_site );
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite& asnMission = *asnMsg.mission.u.mission_pion_asa_mistral_defendre_site;

    NET_ASN_Tools::Delete( asnMission.site );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
