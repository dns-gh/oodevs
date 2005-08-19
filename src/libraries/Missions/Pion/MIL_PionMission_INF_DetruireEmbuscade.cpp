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
#include "MIL_PionMission_INF_DetruireEmbuscade.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_INF_DetruireEmbuscade::nDIAPositionEmbuscadeIdx_ = 0 ;
int MIL_PionMission_INF_DetruireEmbuscade::nDIAPositionRegroupementIdx_ = 0 ;
int MIL_PionMission_INF_DetruireEmbuscade::nDIAEnnemiIdx_ = 0 ;
int MIL_PionMission_INF_DetruireEmbuscade::nDIANatureObstacleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireEmbuscade::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_DetruireEmbuscade::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionEmbuscadeIdx_ = DEC_Tools::InitializeDIAField( "positionEmbuscade_", diaType );
    nDIAPositionRegroupementIdx_ = DEC_Tools::InitializeDIAField( "positionRegroupement_", diaType );
    nDIAEnnemiIdx_ = DEC_Tools::InitializeDIAField( "ennemi_", diaType );
    nDIANatureObstacleIdx_ = DEC_Tools::InitializeDIAField( "natureObstacle_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireEmbuscade constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_INF_DetruireEmbuscade::MIL_PionMission_INF_DetruireEmbuscade( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireEmbuscade destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_INF_DetruireEmbuscade::~MIL_PionMission_INF_DetruireEmbuscade()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireEmbuscade::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_DetruireEmbuscade::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_DetruireEmbuscade& asnMission = *asnMsg.mission.u.mission_pion_inf_detruire_embuscade;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_embuscade, positionEmbuscade_, GetVariable( nDIAPositionEmbuscadeIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_regroupement, positionRegroupement_, GetVariable( nDIAPositionRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgentKnowledge( asnMission.ennemi, GetVariable( nDIAEnnemiIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.nature_obstacle, GetVariable( nDIANatureObstacleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireEmbuscade::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_DetruireEmbuscade::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( positionEmbuscade_, GetVariable( nDIAPositionEmbuscadeIdx_ ) );
    NET_ASN_Tools::ResetPoint( positionRegroupement_, GetVariable( nDIAPositionRegroupementIdx_ ) );
    NET_ASN_Tools::ResetAgentKnowledge( GetVariable( nDIAEnnemiIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIANatureObstacleIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireEmbuscade::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_DetruireEmbuscade::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_DetruireEmbuscade& mission = static_cast< MIL_PionMission_INF_DetruireEmbuscade& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionEmbuscadeIdx_ ), positionEmbuscade_, GetVariable( nDIAPositionEmbuscadeIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionRegroupementIdx_ ), positionRegroupement_, GetVariable( nDIAPositionRegroupementIdx_ ) );
    NET_ASN_Tools::CopyAgentKnowledge( mission.GetVariable( nDIAEnnemiIdx_ ), GetVariable( nDIAEnnemiIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIANatureObstacleIdx_ ), GetVariable( nDIANatureObstacleIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireEmbuscade::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_DetruireEmbuscade::Terminate()
{
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIANatureObstacleIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireEmbuscade::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_DetruireEmbuscade::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_DetruireEmbuscade& asnMission = *new ASN1T_Mission_Pion_INF_DetruireEmbuscade();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_detruire_embuscade;
    asnMsg.mission.u.mission_pion_inf_detruire_embuscade  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionEmbuscadeIdx_ ), asnMission.position_embuscade );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionRegroupementIdx_ ), asnMission.position_regroupement );
    NET_ASN_Tools::CopyAgentKnowledge( GetVariable( nDIAEnnemiIdx_ ), asnMission.ennemi, pion_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIANatureObstacleIdx_ ), asnMission.nature_obstacle );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireEmbuscade::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_DetruireEmbuscade::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_detruire_embuscade );
    ASN1T_Mission_Pion_INF_DetruireEmbuscade& asnMission = *asnMsg.mission.u.mission_pion_inf_detruire_embuscade;

    NET_ASN_Tools::Delete( asnMission.position_embuscade );
    NET_ASN_Tools::Delete( asnMission.position_regroupement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
