// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:3 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_INF_Soutenir.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_INF_Soutenir::nDIAUniteASoutenirIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Soutenir::InitializeDIA
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_Soutenir::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUniteASoutenirIdx_ = DEC_Tools::InitializeDIAField( "uniteASoutenir_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Soutenir constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Soutenir::MIL_PionMission_INF_Soutenir( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Soutenir destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Soutenir::~MIL_PionMission_INF_Soutenir()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Soutenir::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_Soutenir::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_Soutenir& asnMission = *asnMsg.mission.u.mission_pion_inf_soutenir;
    if( !NET_ASN_Tools::CopyAgent( asnMission.unite_a_soutenir, GetVariable( nDIAUniteASoutenirIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Soutenir::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_Soutenir::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAgent( GetVariable( nDIAUniteASoutenirIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Soutenir::Initialize
// Created: 2005-6-28 - 14:3:3
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_Soutenir::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_Soutenir& mission = static_cast< MIL_PionMission_INF_Soutenir& >( missionTmp );

    NET_ASN_Tools::CopyAgent( mission.GetVariable( nDIAUniteASoutenirIdx_ ), GetVariable( nDIAUniteASoutenirIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Soutenir::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Soutenir::Terminate()
{
    NET_ASN_Tools::ResetAgent( GetVariable( nDIAUniteASoutenirIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Soutenir::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Soutenir::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_Soutenir& asnMission = *new ASN1T_Mission_Pion_INF_Soutenir();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_soutenir;
    asnMsg.mission.u.mission_pion_inf_soutenir  = &asnMission;

    NET_ASN_Tools::CopyAgent( GetVariable( nDIAUniteASoutenirIdx_ ), asnMission.unite_a_soutenir );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Soutenir::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Soutenir::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_soutenir );
    ASN1T_Mission_Pion_INF_Soutenir& asnMission = *asnMsg.mission.u.mission_pion_inf_soutenir;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
