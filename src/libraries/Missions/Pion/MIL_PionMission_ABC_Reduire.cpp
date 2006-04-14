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
#include "MIL_PionMission_ABC_Reduire.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_Reduire::nDIAUniteAReduireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Reduire::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_Reduire::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUniteAReduireIdx_ = DEC_Tools::InitializeDIAField( "uniteAReduire_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Reduire constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_Reduire::MIL_PionMission_ABC_Reduire( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Reduire destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_Reduire::~MIL_PionMission_ABC_Reduire()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Reduire::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_Reduire::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_Reduire& asnMission = *asnMsg.mission.u.mission_pion_abc_reduire;
    if( !NET_ASN_Tools::CopyAgentKnowledge( asnMission.unite_a_reduire, GetVariable( nDIAUniteAReduireIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Reduire::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_Reduire::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Reduire::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_Reduire::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_Reduire& mission = static_cast< MIL_PionMission_ABC_Reduire& >( missionTmp );

    NET_ASN_Tools::CopyAgentKnowledge( mission.GetVariable( nDIAUniteAReduireIdx_ ), GetVariable( nDIAUniteAReduireIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Reduire::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Reduire::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Reduire::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Reduire::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_Reduire& asnMission = *new ASN1T_Mission_Pion_ABC_Reduire();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_reduire;
    asnMsg.mission.u.mission_pion_abc_reduire  = &asnMission;

    NET_ASN_Tools::CopyAgentKnowledge( GetVariable( nDIAUniteAReduireIdx_ ), asnMission.unite_a_reduire, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Reduire::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Reduire::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_reduire );
    ASN1T_Mission_Pion_ABC_Reduire& asnMission = *asnMsg.mission.u.mission_pion_abc_reduire;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
