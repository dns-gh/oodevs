// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:44 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_LOG_ArmerPIA.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_LOG_ArmerPIA::nDIAPointIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ArmerPIA::InitializeDIA
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_ArmerPIA::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointIdx_ = DEC_Tools::InitializeDIAField( "point_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ArmerPIA constructor
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_ArmerPIA::MIL_PionMission_LOG_ArmerPIA( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ArmerPIA destructor
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_ArmerPIA::~MIL_PionMission_LOG_ArmerPIA()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ArmerPIA::Initialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_ArmerPIA::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_LOG_ArmerPIA& asnMission = *asnMsg.mission.u.mission_pion_log_armer_pia;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point, point_, GetVariable( nDIAPointIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ArmerPIA::Initialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_ArmerPIA::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( point_, GetVariable( nDIAPointIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ArmerPIA::Initialize
// Created: 2005-08-17 - 16:30:44
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_ArmerPIA::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_ArmerPIA& mission = static_cast< MIL_PionMission_LOG_ArmerPIA& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointIdx_ ), point_, GetVariable( nDIAPointIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ArmerPIA::Terminate
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_ArmerPIA::Terminate()
{
    NET_ASN_Tools::ResetPoint( point_, GetVariable( nDIAPointIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ArmerPIA::Serialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_ArmerPIA::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_ArmerPIA& asnMission = *new ASN1T_Mission_Pion_LOG_ArmerPIA();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_armer_pia;
    asnMsg.mission.u.mission_pion_log_armer_pia  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointIdx_ ), asnMission.point );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_ArmerPIA::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_ArmerPIA::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_armer_pia );
    ASN1T_Mission_Pion_LOG_ArmerPIA& asnMission = *asnMsg.mission.u.mission_pion_log_armer_pia;

    NET_ASN_Tools::Delete( asnMission.point );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
