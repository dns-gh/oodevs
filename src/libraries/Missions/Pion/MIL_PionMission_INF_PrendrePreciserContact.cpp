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

#include "Missions_pch.h"
#include "MIL_PionMission_INF_PrendrePreciserContact.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_INF_PrendrePreciserContact::nDIAEnnemiIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_PrendrePreciserContact::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_PrendrePreciserContact::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAEnnemiIdx_ = DEC_Tools::InitializeDIAField( "ennemi_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_PrendrePreciserContact constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_PrendrePreciserContact::MIL_PionMission_INF_PrendrePreciserContact( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_PrendrePreciserContact destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_PrendrePreciserContact::~MIL_PionMission_INF_PrendrePreciserContact()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_PrendrePreciserContact::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_PrendrePreciserContact::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_PrendrePreciserContact& asnMission = *asnMsg.mission.u.mission_pion_inf_prendre_preciser_contact;
    if( !NET_ASN_Tools::CopyAgentKnowledge( asnMission.ennemi, GetVariable( nDIAEnnemiIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_PrendrePreciserContact::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_PrendrePreciserContact::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_PrendrePreciserContact::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_PrendrePreciserContact::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_PrendrePreciserContact& mission = static_cast< MIL_PionMission_INF_PrendrePreciserContact& >( missionTmp );

    NET_ASN_Tools::CopyAgentKnowledge( mission.GetVariable( nDIAEnnemiIdx_ ), GetVariable( nDIAEnnemiIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_PrendrePreciserContact::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_PrendrePreciserContact::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_PrendrePreciserContact::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_PrendrePreciserContact::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_PrendrePreciserContact& asnMission = *new ASN1T_Mission_Pion_INF_PrendrePreciserContact();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_prendre_preciser_contact;
    asnMsg.mission.u.mission_pion_inf_prendre_preciser_contact  = &asnMission;

    NET_ASN_Tools::CopyAgentKnowledge( GetVariable( nDIAEnnemiIdx_ ), asnMission.ennemi, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_PrendrePreciserContact::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_PrendrePreciserContact::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_prendre_preciser_contact );
    ASN1T_Mission_Pion_INF_PrendrePreciserContact& asnMission = *asnMsg.mission.u.mission_pion_inf_prendre_preciser_contact;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
