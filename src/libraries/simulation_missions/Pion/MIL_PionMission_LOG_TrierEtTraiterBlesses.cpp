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
#include "MIL_PionMission_LOG_TrierEtTraiterBlesses.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_LOG_TrierEtTraiterBlesses::nDIABlessuresTraiteesIdx_ = 0 ;
int MIL_PionMission_LOG_TrierEtTraiterBlesses::nDIAPositionDeploiementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterBlesses::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_TrierEtTraiterBlesses::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIABlessuresTraiteesIdx_ = DEC_Tools::InitializeDIAField( "blessuresTraitees_", diaType );
    nDIAPositionDeploiementIdx_ = DEC_Tools::InitializeDIAField( "positionDeploiement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterBlesses constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_TrierEtTraiterBlesses::MIL_PionMission_LOG_TrierEtTraiterBlesses( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterBlesses destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_TrierEtTraiterBlesses::~MIL_PionMission_LOG_TrierEtTraiterBlesses()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterBlesses::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_TrierEtTraiterBlesses::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses& asnMission = *asnMsg.mission.u.mission_pion_log_trier_et_traiter_blesses;
    if( !NET_ASN_Tools::CopyMedicalPriorities( asnMission.blessures_traitees, GetVariable( nDIABlessuresTraiteesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_deploiement, GetVariable( nDIAPositionDeploiementIdx_ ), asnMission.m.position_deploiementPresent ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterBlesses::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_TrierEtTraiterBlesses::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterBlesses::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_TrierEtTraiterBlesses::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_TrierEtTraiterBlesses& mission = static_cast< MIL_PionMission_LOG_TrierEtTraiterBlesses& >( missionTmp );

    NET_ASN_Tools::CopyMedicalPriorities( mission.GetVariable( nDIABlessuresTraiteesIdx_ ), GetVariable( nDIABlessuresTraiteesIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionDeploiementIdx_ ), GetVariable( nDIAPositionDeploiementIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterBlesses::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TrierEtTraiterBlesses::Terminate()
{
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterBlesses::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TrierEtTraiterBlesses::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_trier_et_traiter_blesses;
    asnMsg.mission.u.mission_pion_log_trier_et_traiter_blesses  = &asnMission;

    NET_ASN_Tools::CopyMedicalPriorities( GetVariable( nDIABlessuresTraiteesIdx_ ), asnMission.blessures_traitees );
    if( NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionDeploiementIdx_ ), asnMission.position_deploiement ) )
        asnMission.m.position_deploiementPresent = 1;

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TrierEtTraiterBlesses::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TrierEtTraiterBlesses::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_trier_et_traiter_blesses );
    ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses& asnMission = *asnMsg.mission.u.mission_pion_log_trier_et_traiter_blesses;

    NET_ASN_Tools::Delete( asnMission.blessures_traitees );
    if( asnMission.m.position_deploiementPresent )
        NET_ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
