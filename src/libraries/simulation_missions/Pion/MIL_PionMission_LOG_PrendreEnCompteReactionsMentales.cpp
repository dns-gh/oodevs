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
#include "MIL_PionMission_LOG_PrendreEnCompteReactionsMentales.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::nDIAPositionDeploiementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionDeploiementIdx_ = DEC_Tools::InitializeDIAField( "positionDeploiement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PrendreEnCompteReactionsMentales constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::MIL_PionMission_LOG_PrendreEnCompteReactionsMentales( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PrendreEnCompteReactionsMentales destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::~MIL_PionMission_LOG_PrendreEnCompteReactionsMentales()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales& asnMission = *asnMsg.mission.u.mission_pion_log_prendre_en_compte_reactions_mentales;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_deploiement, GetVariable( nDIAPositionDeploiementIdx_ ), asnMission.m.position_deploiementPresent ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_PrendreEnCompteReactionsMentales& mission = static_cast< MIL_PionMission_LOG_PrendreEnCompteReactionsMentales& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionDeploiementIdx_ ), GetVariable( nDIAPositionDeploiementIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales& asnMission = *new ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_prendre_en_compte_reactions_mentales;
    asnMsg.mission.u.mission_pion_log_prendre_en_compte_reactions_mentales  = &asnMission;

    if( NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionDeploiementIdx_ ), asnMission.position_deploiement ) )
        asnMission.m.position_deploiementPresent = 1;

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_PrendreEnCompteReactionsMentales::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_prendre_en_compte_reactions_mentales );
    ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales& asnMission = *asnMsg.mission.u.mission_pion_log_prendre_en_compte_reactions_mentales;

    if( asnMission.m.position_deploiementPresent )
        NET_ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
