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
#include "MIL_PionMission_ABC_Fixer.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_Fixer::nDIAUniteAFixerIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Fixer::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_Fixer::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUniteAFixerIdx_ = DEC_Tools::InitializeDIAField( "uniteAFixer_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Fixer constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_Fixer::MIL_PionMission_ABC_Fixer( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Fixer destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_Fixer::~MIL_PionMission_ABC_Fixer()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Fixer::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_Fixer::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_Fixer& asnMission = *asnMsg.mission.u.mission_pion_abc_fixer;
    if( !NET_ASN_Tools::CopyAgentKnowledge( asnMission.unite_a_fixer, GetVariable( nDIAUniteAFixerIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Fixer::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_Fixer::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Fixer::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_Fixer::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_Fixer& mission = static_cast< MIL_PionMission_ABC_Fixer& >( missionTmp );

    NET_ASN_Tools::CopyAgentKnowledge( mission.GetVariable( nDIAUniteAFixerIdx_ ), GetVariable( nDIAUniteAFixerIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Fixer::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Fixer::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Fixer::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Fixer::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_Fixer& asnMission = *new ASN1T_Mission_Pion_ABC_Fixer();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_fixer;
    asnMsg.mission.u.mission_pion_abc_fixer  = &asnMission;

    NET_ASN_Tools::CopyAgentKnowledge( GetVariable( nDIAUniteAFixerIdx_ ), asnMission.unite_a_fixer, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Fixer::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Fixer::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_fixer );
    ASN1T_Mission_Pion_ABC_Fixer& asnMission = *asnMsg.mission.u.mission_pion_abc_fixer;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
