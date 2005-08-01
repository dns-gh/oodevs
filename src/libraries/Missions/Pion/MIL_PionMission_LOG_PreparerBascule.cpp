// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_LOG_PreparerBascule.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_LOG_PreparerBascule::nDIAAutomatePourBasculeIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PreparerBascule::InitializeDIA
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_PreparerBascule::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAAutomatePourBasculeIdx_ = DEC_Tools::InitializeDIAField( "automatePourBascule_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PreparerBascule constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_PreparerBascule::MIL_PionMission_LOG_PreparerBascule( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PreparerBascule destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_PreparerBascule::~MIL_PionMission_LOG_PreparerBascule()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PreparerBascule::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_PreparerBascule::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_LOG_PreparerBascule& asnMission = *asnMsg.mission.u.mission_pion_log_preparer_bascule;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.automate_pour_bascule, GetVariable( nDIAAutomatePourBasculeIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PreparerBascule::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_PreparerBascule::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAutomate( GetVariable( nDIAAutomatePourBasculeIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PreparerBascule::Initialize
// Created: 2005-08-01 - 11:23:53
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_PreparerBascule::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_PreparerBascule& mission = static_cast< MIL_PionMission_LOG_PreparerBascule& >( missionTmp );

    NET_ASN_Tools::CopyAutomate( mission.GetVariable( nDIAAutomatePourBasculeIdx_ ), GetVariable( nDIAAutomatePourBasculeIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PreparerBascule::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_PreparerBascule::Terminate()
{
    NET_ASN_Tools::ResetAutomate( GetVariable( nDIAAutomatePourBasculeIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PreparerBascule::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_PreparerBascule::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_PreparerBascule& asnMission = *new ASN1T_Mission_Pion_LOG_PreparerBascule();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_preparer_bascule;
    asnMsg.mission.u.mission_pion_log_preparer_bascule  = &asnMission;

    NET_ASN_Tools::CopyAutomate( GetVariable( nDIAAutomatePourBasculeIdx_ ), asnMission.automate_pour_bascule );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_PreparerBascule::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_PreparerBascule::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_preparer_bascule );
    ASN1T_Mission_Pion_LOG_PreparerBascule& asnMission = *asnMsg.mission.u.mission_pion_log_preparer_bascule;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
