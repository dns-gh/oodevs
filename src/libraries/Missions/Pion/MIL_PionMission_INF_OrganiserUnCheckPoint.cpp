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
#include "MIL_PionMission_INF_OrganiserUnCheckPoint.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_INF_OrganiserUnCheckPoint::nDIAPointInstallationIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_OrganiserUnCheckPoint::InitializeDIA
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_OrganiserUnCheckPoint::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointInstallationIdx_ = DEC_Tools::InitializeDIAField( "pointInstallation_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_OrganiserUnCheckPoint constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_INF_OrganiserUnCheckPoint::MIL_PionMission_INF_OrganiserUnCheckPoint( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_OrganiserUnCheckPoint destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_INF_OrganiserUnCheckPoint::~MIL_PionMission_INF_OrganiserUnCheckPoint()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_OrganiserUnCheckPoint::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_OrganiserUnCheckPoint::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint& asnMission = *asnMsg.mission.u.mission_pion_inf_organiser_un_check_point;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_installation, pointInstallation_, GetVariable( nDIAPointInstallationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_OrganiserUnCheckPoint::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_OrganiserUnCheckPoint::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( pointInstallation_, GetVariable( nDIAPointInstallationIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_OrganiserUnCheckPoint::Initialize
// Created: 2005-08-01 - 11:23:53
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_OrganiserUnCheckPoint::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_OrganiserUnCheckPoint& mission = static_cast< MIL_PionMission_INF_OrganiserUnCheckPoint& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointInstallationIdx_ ), pointInstallation_, GetVariable( nDIAPointInstallationIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_OrganiserUnCheckPoint::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_OrganiserUnCheckPoint::Terminate()
{
    NET_ASN_Tools::ResetPoint( pointInstallation_, GetVariable( nDIAPointInstallationIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_OrganiserUnCheckPoint::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_OrganiserUnCheckPoint::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint& asnMission = *new ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_organiser_un_check_point;
    asnMsg.mission.u.mission_pion_inf_organiser_un_check_point  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointInstallationIdx_ ), asnMission.point_installation );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_OrganiserUnCheckPoint::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_OrganiserUnCheckPoint::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_organiser_un_check_point );
    ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint& asnMission = *asnMsg.mission.u.mission_pion_inf_organiser_un_check_point;

    NET_ASN_Tools::Delete( asnMission.point_installation );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
