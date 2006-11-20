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
#include "MIL_PionMission_TRANS_EtablirGererLiaison.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_TRANS_EtablirGererLiaison::nDIAPositionDeploiementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_TRANS_EtablirGererLiaison::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_TRANS_EtablirGererLiaison::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionDeploiementIdx_ = DEC_Tools::InitializeDIAField( "positionDeploiement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_TRANS_EtablirGererLiaison constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_TRANS_EtablirGererLiaison::MIL_PionMission_TRANS_EtablirGererLiaison( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_TRANS_EtablirGererLiaison destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_TRANS_EtablirGererLiaison::~MIL_PionMission_TRANS_EtablirGererLiaison()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_TRANS_EtablirGererLiaison::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_TRANS_EtablirGererLiaison::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_TRANS_EtablirGererLiaison& asnMission = *asnMsg.mission.u.mission_pion_trans_etablir_gerer_liaison;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_deploiement, GetVariable( nDIAPositionDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_TRANS_EtablirGererLiaison::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_TRANS_EtablirGererLiaison::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_TRANS_EtablirGererLiaison::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_TRANS_EtablirGererLiaison::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_TRANS_EtablirGererLiaison& mission = static_cast< MIL_PionMission_TRANS_EtablirGererLiaison& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionDeploiementIdx_ ), GetVariable( nDIAPositionDeploiementIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_TRANS_EtablirGererLiaison::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_TRANS_EtablirGererLiaison::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_TRANS_EtablirGererLiaison& asnMission = *new ASN1T_Mission_Pion_TRANS_EtablirGererLiaison();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_trans_etablir_gerer_liaison;
    asnMsg.mission.u.mission_pion_trans_etablir_gerer_liaison  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionDeploiementIdx_ ), asnMission.position_deploiement );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_TRANS_EtablirGererLiaison::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_TRANS_EtablirGererLiaison::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_trans_etablir_gerer_liaison );
    ASN1T_Mission_Pion_TRANS_EtablirGererLiaison& asnMission = *asnMsg.mission.u.mission_pion_trans_etablir_gerer_liaison;

    NET_ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
