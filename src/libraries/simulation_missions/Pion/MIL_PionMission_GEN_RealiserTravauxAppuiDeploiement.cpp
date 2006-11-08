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
#include "MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::nDIATravauxIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIATravauxIdx_ = DEC_Tools::InitializeDIAField( "travaux_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::~MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_RealiserTravauxAppuiDeploiement& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_travaux_appui_deploiement;
    if( !NET_ASN_Tools::CopyGenObjectList( asnMission.travaux, GetVariable( nDIATravauxIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement& mission = static_cast< MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement& >( missionTmp );

    NET_ASN_Tools::CopyGenObjectList( mission.GetVariable( nDIATravauxIdx_ ), GetVariable( nDIATravauxIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::Terminate()
{
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_RealiserTravauxAppuiDeploiement& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserTravauxAppuiDeploiement();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_realiser_travaux_appui_deploiement;
    asnMsg.mission.u.mission_pion_gen_realiser_travaux_appui_deploiement  = &asnMission;

    NET_ASN_Tools::CopyGenObjectList( GetVariable( nDIATravauxIdx_ ), asnMission.travaux );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserTravauxAppuiDeploiement::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_realiser_travaux_appui_deploiement );
    ASN1T_Mission_Pion_GEN_RealiserTravauxAppuiDeploiement& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_travaux_appui_deploiement;

    NET_ASN_Tools::Delete( asnMission.travaux );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
