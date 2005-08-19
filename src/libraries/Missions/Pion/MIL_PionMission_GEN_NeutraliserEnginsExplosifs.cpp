// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_GEN_NeutraliserEnginsExplosifs.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_NeutraliserEnginsExplosifs::nDIAObstacleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_NeutraliserEnginsExplosifs::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_NeutraliserEnginsExplosifs::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAObstacleIdx_ = DEC_Tools::InitializeDIAField( "obstacle_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_NeutraliserEnginsExplosifs constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_NeutraliserEnginsExplosifs::MIL_PionMission_GEN_NeutraliserEnginsExplosifs( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_NeutraliserEnginsExplosifs destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_NeutraliserEnginsExplosifs::~MIL_PionMission_GEN_NeutraliserEnginsExplosifs()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_NeutraliserEnginsExplosifs::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_NeutraliserEnginsExplosifs::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs& asnMission = *asnMsg.mission.u.mission_pion_gen_neutraliser_engins_explosifs;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.obstacle, GetVariable( nDIAObstacleIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_NeutraliserEnginsExplosifs::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_NeutraliserEnginsExplosifs::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetObjectKnowledge( GetVariable( nDIAObstacleIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_NeutraliserEnginsExplosifs::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_NeutraliserEnginsExplosifs::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_NeutraliserEnginsExplosifs& mission = static_cast< MIL_PionMission_GEN_NeutraliserEnginsExplosifs& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIAObstacleIdx_ ), GetVariable( nDIAObstacleIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_NeutraliserEnginsExplosifs::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_NeutraliserEnginsExplosifs::Terminate()
{
    NET_ASN_Tools::ResetObjectKnowledge( GetVariable( nDIAObstacleIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_NeutraliserEnginsExplosifs::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_NeutraliserEnginsExplosifs::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs& asnMission = *new ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_neutraliser_engins_explosifs;
    asnMsg.mission.u.mission_pion_gen_neutraliser_engins_explosifs  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIAObstacleIdx_ ), asnMission.obstacle, pion_.GetKnowledgeGroup().GetKSQuerier() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_NeutraliserEnginsExplosifs::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_NeutraliserEnginsExplosifs::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_neutraliser_engins_explosifs );
    ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs& asnMission = *asnMsg.mission.u.mission_pion_gen_neutraliser_engins_explosifs;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
