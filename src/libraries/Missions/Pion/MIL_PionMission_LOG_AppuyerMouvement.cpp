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
#include "MIL_PionMission_LOG_AppuyerMouvement.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_LOG_AppuyerMouvement::nDIAUniteAAppuyerIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerMouvement::InitializeDIA
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_AppuyerMouvement::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUniteAAppuyerIdx_ = DEC_Tools::InitializeDIAField( "uniteAAppuyer_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerMouvement constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_AppuyerMouvement::MIL_PionMission_LOG_AppuyerMouvement( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerMouvement destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_AppuyerMouvement::~MIL_PionMission_LOG_AppuyerMouvement()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerMouvement::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_AppuyerMouvement::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_LOG_AppuyerMouvement& asnMission = *asnMsg.mission.u.mission_pion_log_appuyer_mouvement;
    if( !NET_ASN_Tools::CopyAgent( asnMission.unite_a_appuyer, GetVariable( nDIAUniteAAppuyerIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerMouvement::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_AppuyerMouvement::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAgent( GetVariable( nDIAUniteAAppuyerIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerMouvement::Initialize
// Created: 2005-6-28 - 14:3:3
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_AppuyerMouvement::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_AppuyerMouvement& mission = static_cast< MIL_PionMission_LOG_AppuyerMouvement& >( missionTmp );

    NET_ASN_Tools::CopyAgent( mission.GetVariable( nDIAUniteAAppuyerIdx_ ), GetVariable( nDIAUniteAAppuyerIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerMouvement::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_AppuyerMouvement::Terminate()
{
    NET_ASN_Tools::ResetAgent( GetVariable( nDIAUniteAAppuyerIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerMouvement::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_AppuyerMouvement::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_AppuyerMouvement& asnMission = *new ASN1T_Mission_Pion_LOG_AppuyerMouvement();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_appuyer_mouvement;
    asnMsg.mission.u.mission_pion_log_appuyer_mouvement  = &asnMission;

    NET_ASN_Tools::CopyAgent( GetVariable( nDIAUniteAAppuyerIdx_ ), asnMission.unite_a_appuyer );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_AppuyerMouvement::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_AppuyerMouvement::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_appuyer_mouvement );
    ASN1T_Mission_Pion_LOG_AppuyerMouvement& asnMission = *asnMsg.mission.u.mission_pion_log_appuyer_mouvement;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
