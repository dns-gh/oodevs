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
#include "MIL_PionMission_ABC_ArmerPIA.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_ArmerPIA::nDIAPiaIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPIA::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_ArmerPIA::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPiaIdx_ = DEC_Tools::InitializeDIAField( "pia_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPIA constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_ArmerPIA::MIL_PionMission_ABC_ArmerPIA( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPIA destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_ArmerPIA::~MIL_PionMission_ABC_ArmerPIA()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPIA::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_ArmerPIA::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_ArmerPIA& asnMission = *asnMsg.mission.u.mission_pion_abc_armer_pia;
    if( !NET_ASN_Tools::CopyPoint( asnMission.pia, pia_, GetVariable( nDIAPiaIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPIA::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_ArmerPIA::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( pia_, GetVariable( nDIAPiaIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPIA::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_ArmerPIA::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_ArmerPIA& mission = static_cast< MIL_PionMission_ABC_ArmerPIA& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPiaIdx_ ), pia_, GetVariable( nDIAPiaIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPIA::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ArmerPIA::Terminate()
{
    NET_ASN_Tools::ResetPoint( pia_, GetVariable( nDIAPiaIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPIA::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ArmerPIA::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_ArmerPIA& asnMission = *new ASN1T_Mission_Pion_ABC_ArmerPIA();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_armer_pia;
    asnMsg.mission.u.mission_pion_abc_armer_pia  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPiaIdx_ ), asnMission.pia );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPIA::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ArmerPIA::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_armer_pia );
    ASN1T_Mission_Pion_ABC_ArmerPIA& asnMission = *asnMsg.mission.u.mission_pion_abc_armer_pia;

    NET_ASN_Tools::Delete( asnMission.pia );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
